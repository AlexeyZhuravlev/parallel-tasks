#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "field_generation.h"
#include "parallel_worker.h"

char** field;
int n, m, k, number_of_nodes, rank;

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Specify sizes of field and number of iterations\n");
        return 0;
    }
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    k = atoi(argv[3]);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_nodes);
    if (number_of_nodes < 2) {
        printf("At least two nodes is required: one master and one slave\n");
        MPI_Finalize();
        return 0;
    }
    if (n < number_of_nodes - 1) {
        printf("Too small field for so much processes\n");
        MPI_Finalize();
        return 0;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double time_point1, time_point2, parallel, not_parallel;
        int number_of_slaves = number_of_nodes - 1;
        struct segment_t* segments = (struct segment_t*)malloc(number_of_slaves * 
                                                           sizeof(struct segment_t));
        distribute_segments(segments, n, number_of_slaves);
        generate_field_at_random(n, m, &field);
        time_point1 = MPI_Wall();
        send_field(segments, field, n, m, number_of_slaves);
        receive_field(segments, field, n, m, number_of_slaves);
        time_point2 = MPI_Wall();
        parallel = time_point2 - time_point1;
        printf("Elapsed calculations time: %f\n", parallel);
        time_point1 = MPI_Wall();
        calculate_not_parallel(field, n, m, k);
        time_point2 = MPI_Wall();
        not_parallel = time_point2 - time_point1;
        printf("Not parallel time: %f\n", not_parallel);
        printf("Acceleration: %f\n", not_parallel / parallel);
    } else {
        slave_computations(rank, m, k, number_of_nodes - 1);
    }
    MPI_Finalize();
    return 0;
}
