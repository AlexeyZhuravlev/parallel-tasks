#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>
#include <unistd.h>
#include "field_generation.h"
#include "parallel_worker.h"
#include "non_parallel_life.h"
#include "two_dim_array_operations.h"

char** field;
char** second_field;
int n, m, k, number_of_nodes, rank;

int main(int argc, char** argv) {
    double parallel, not_parallel, time_point1, time_point2;
    time_point1 = 0;
    if (argc < 4) {
        printf("Specify sizes of field and number of iterations\n");
        return 0;
    }
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    k = atoi(argv[3]);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_nodes);
    if (n < number_of_nodes) {
        printf("Too small field for so much processes\n");
        MPI_Finalize();
        return 0;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        generate_field_at_random(n, m, &field);
        allocate_two_dimentional_array(&second_field, n, m);
        copy_two_dimentional_array(field, second_field, n, m);
        time_point1 = MPI_Wtime();
    }
    scatter_field(field, n, m, number_of_nodes, rank);
    calculate_in_parallel(rank, k, number_of_nodes, m);
    gather_field(field, n, m, number_of_nodes, rank);
    if (rank == 0) {
        int i, j;
        time_point2 = MPI_Wtime();
        parallel = time_point2 - time_point1;
        printf("Elapsed calculations time: %f\n", parallel);
        time_point1 = MPI_Wtime();
        calculate_not_parallel(second_field, n, m, k);
        time_point2 = MPI_Wtime();
        not_parallel = time_point2 - time_point1;
        printf("Not parallel time: %f\n", not_parallel);
        printf("Acceleration: %f\n", not_parallel / parallel);
        for (i = 0; i < n; i++)
            for (j = 0; j < m; j++)
                assert(field[i][j] == second_field[i][j]);
        dispose_two_dimentional_array(field, n);
        dispose_two_dimentional_array(second_field, n);
    }
    MPI_Finalize();
    return 0;
}
