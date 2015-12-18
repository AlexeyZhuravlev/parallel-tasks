#include "non_parallel_life.h"
#include "two_dim_array_operations.h"
#include "parallel_worker.h"
#include "string_operations.h"
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

char** local_field;
char** another_field;
int local_n;
struct segment_t* segments;
char* vectorized_field;
char* vectorized_local_field;
int *size, *offset;

void distribute_segments(struct segment_t* segments, int n, int number_of_nodes) {
    int i;
    int k = n / number_of_nodes;
    int border = n % number_of_nodes;
    int last = 0;
    for (i = 0; i < number_of_nodes; i++) {
        segments[i].l = last;
        segments[i].r = last + k - 1;
        if (border) {
            segments[i].r++;
            border--;
        }
        last = segments[i].r + 1;
    }
}

void scatter_field(char** field, int n, int m, int number_of_nodes, int rank) {
    int i, count, sum, local_size;
    segments = (struct segment_t*)malloc(number_of_nodes * sizeof(struct segment_t));
    distribute_segments(segments, n, number_of_nodes);
    if (rank == 0) {
        vectorized_field = (char*)malloc(n * m * sizeof(char));
        size = (int*)malloc(number_of_nodes * sizeof(int));
        offset = (int*)malloc(number_of_nodes * sizeof(int));
        translate_matrix_to_vector(field, vectorized_field, n, m);
        sum = 0;
        for (i = 0; i < number_of_nodes; i++) {
            count = segments[i].r - segments[i].l + 1;
            size[i] = count * m;
            offset[i] = sum * m; 
            sum += count;
        }
    }
    local_n = segments[rank].r - segments[rank].l + 1;
    local_size = local_n * m;
    vectorized_local_field = (char*)malloc(local_size * sizeof(char));
    MPI_Scatterv(vectorized_field, size, offset, MPI_CHAR, 
                 vectorized_local_field, local_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    allocate_two_dimentional_array(&local_field, local_n + 2, m);
    allocate_two_dimentional_array(&another_field, local_n + 2, m);
    translate_vector_to_matrix(vectorized_local_field, local_field + 1, local_n, m); 
    local_n += 2;
}

void calculate_in_parallel(int rank, int number_of_iterations, int number_of_nodes, int m) {
    int i, j, k, n;
    char* borders = NULL;
    n = local_n;
    if (rank == 0) {
        borders = (char*)malloc((number_of_nodes + 1) * m * sizeof(char));
    }
    for (k = 0; k < number_of_iterations; k++) {
        MPI_Gather(local_field[1], m, MPI_CHAR, 
                   borders, m, MPI_CHAR, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            memcpy(borders + number_of_nodes * m, borders, m * sizeof(char));
        }
        MPI_Scatter(borders + m, m, MPI_CHAR,
                    local_field[n - 1], m, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Gather(local_field[n - 2], m, MPI_CHAR,
                   borders + m, m, MPI_CHAR, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            memcpy(borders, borders + number_of_nodes * m, m * sizeof(char));
        }
        MPI_Scatter(borders, m, MPI_CHAR,
                    local_field[0], m, MPI_CHAR, 0, MPI_COMM_WORLD);
        for (i = 1; i < n - 1; i++) {
            for (j = 0; j < m; j++) {
                recalculate_on_coordinate(local_field, another_field, n, m, i, j);
            }
        }
        two_dimentional_array_swap(&local_field, &another_field);
    }
    if (rank == 0) {
        free(borders);
    }
}

void gather_field(char** field, int n, int m, int number_of_nodes, int rank) {
    local_n -= 2;
    translate_matrix_to_vector(local_field + 1, vectorized_local_field, local_n, m);
    MPI_Gatherv(vectorized_local_field, local_n * m, MPI_CHAR,
                vectorized_field, size, offset, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        translate_vector_to_matrix(vectorized_field, field, n, m);
        free(vectorized_field);
        free(size);
        free(offset);
    }
    free(vectorized_local_field);
    free(segments);
    dispose_two_dimentional_array(local_field, local_n + 2);
    dispose_two_dimentional_array(another_field, local_n + 2);
}
