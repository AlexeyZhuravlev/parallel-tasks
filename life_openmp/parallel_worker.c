#define _GNU_SOURCE

#include "non_parallel_life.h"
#include "two_dim_array_operations.h"
#include "string_operations.h"
#include <stdlib.h>
#include <stdio.h>

char** field;
char** new_field;
int n, m;
int total_number_of_threads;
int total_number_of_iterations;
int number_of_finished_treads;
int finished;

char stop_immediately;

void finalize() {
    if (total_number_of_iterations % 2 == 1) {
        two_dimentional_array_swap(&field, &new_field);
        copy_two_dimentional_array(new_field, field, n, m);
    }
    dispose_two_dimentional_array(new_field, n);
}

void life_computator() {
int step, i, j;
#pragma omp parallel num_threads(total_number_of_threads) private(step, i, j) 
    {
        for (step = 0; step < total_number_of_iterations; step++) {
#pragma omp for schedule (static)
            for (i = 0; i < n; i++) {
                for (j = 0; j < m; j++) {
                    recalculate_on_coordinate(field, new_field, n, m, i, j); 
                 }
            }
#pragma omp single
            {
                two_dimentional_array_swap(&field, &new_field);
                if (stop_immediately) {
                    total_number_of_iterations = step;
                }
            }
        }
    }
    printf("Calculations were finished on iteration %d\n", total_number_of_iterations);
    finished = 1;
    finalize();
}

void init_parallel_mode() {
    finished = 0;
    stop_immediately = 0;
    allocate_two_dimentional_array(&new_field, n, m);
}

void run_parallel(int number_of_threads, int number_of_iterations, 
                 char** passed_field, int passed_n, int passed_m) {
    n = passed_n;
    m = passed_m;
    if (number_of_threads > n)
        number_of_threads = n;
    field = passed_field;
    total_number_of_threads = number_of_threads;
    total_number_of_iterations = number_of_iterations;
    init_parallel_mode();
    life_computator();
}


void stop() {
    stop_immediately = 1;
}

char is_finished() {
    if (finished) {
        return 1;
    } else {
        return 0;
    }
}
