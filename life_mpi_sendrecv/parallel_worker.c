#include "non_parallel_life.h"
#include "two_dim_array_operations.h"
#include "parallel_worker.h"
#include "string_operations.h"
#include <stdlib.h>
#include <mpi.h>

void distribute_segments(struct segment_t* segments, int n, int number_of_slaves) {
    int i;
    int k = n / number_of_slaves;
    int border = n % number_of_slaves;
    int last = 0;
    for (i = 0; i < number_of_slaves; i++) {
        segments[i].l = last;
        segments[i].r = last + k - 1;
        if (border) {
            segments[i].r++;
            border--;
        }
        last = segments[i].r + 1;
    }
}

void send_field(struct segment_t* segments, char** field, int n, int m, 
                int number_of_slaves) {
    int i;
    for (i = 0; i < number_of_slaves; i++) {
        char* message;
        int count = segments[i].r - segments[i].l + 1;
        translate_matrix_to_vector(field + segments[i].l, &message, count, m);
        MPI_Send(&count, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
        MPI_Send(message, count * m, MPI_CHAR, i + 1, 1, MPI_COMM_WORLD);
        free(message);
    }
}

void receive_field(struct segment_t* segments, char** field, int n, int m, 
                   int number_of_slaves) {
   int i;
   char* buffer;
   for (i = 0; i < number_of_slaves; i++) {
       int buffer_size = (segments[i].r - segments[i].l + 1) * m;
       buffer = (char*)malloc(buffer_size * sizeof(char));
       MPI_Recv(buffer, buffer_size, MPI_CHAR, i + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       translate_vector_to_matrix(buffer, field + segments[i].l, 
                                  segments[i].r - segments[i].l + 1, m);
       free(buffer);
   }
}

void slave_computations(int rank, int m, int number_of_steps, int number_of_slaves) {
    int i, j, k, n, left_rank, right_rank;
    char* buffer;
    char** field;
    char** new_field;
    char continuation_bit = 1;
    MPI_Request request;
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    buffer =(char*)malloc(n * m * sizeof(char));
    MPI_Recv(buffer, n * m, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    n += 2;
    allocate_two_dimentional_array(&field, n, m);
    allocate_two_dimentional_array(&new_field, n, m);
    translate_vector_to_matrix(buffer, field + 1, n - 2, m);
    free(buffer);
    left_rank = rank - 1;
    if (left_rank == 0)
        left_rank = number_of_slaves;
    right_rank = rank + 1;
    if (right_rank > number_of_slaves)
        right_rank = 1;
    if (rank == 1) {
        MPI_Irecv(&continuation_bit, 1, MPI_CHAR, 0, 13, MPI_COMM_WORLD, &request); 
    } else {
        MPI_Irecv(&number_of_steps, 1, MPI_INT, 1, number_of_steps + 1, MPI_COMM_WORLD, &request);
    }
    for (k = 0; k < number_of_steps; k++) {
        if (rank == 1) {
            if (continuation_bit == 0) {
                int process;
                for (process = 2; process < number_of_slaves + 1; process++) {
                    MPI_Ssend(&k, 1, MPI_INT, process, number_of_steps + 1, MPI_COMM_WORLD);
                }
                number_of_steps = k;
            }
        }
        MPI_Sendrecv(field[n - 2], m, MPI_CHAR, right_rank, k, 
                     field[0], m, MPI_CHAR, left_rank, k, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(field[1], m, MPI_CHAR, left_rank, k,
                     field[n - 1], m, MPI_CHAR, right_rank, k, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = 1; i < n - 1; i++) {
            for (j = 0; j < m; j++) {
                recalculate_on_coordinate(field, new_field, n, m, i, j);
            }
        }
        two_dimentional_array_swap(&field, &new_field);
    }
    printf("Node %d finished on step %d\n", rank, number_of_steps);
    MPI_Request_free(&request);
    n -= 2;
    translate_matrix_to_vector(field + 1, &buffer, n, m);
    MPI_Send(buffer, n * m, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    free(buffer);
    n += 2;
    dispose_two_dimentional_array(field, n);
    dispose_two_dimentional_array(new_field, n);
}
