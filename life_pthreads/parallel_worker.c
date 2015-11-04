#define _GNU_SOURCE

#include "non_parallel_life.h"
#include "two_dim_array_operations.h"
#include "string_operations.h"
#include <pthread.h>
#include <stdlib.h>

char** field;
char** new_field;
int n, m;
int total_number_of_threads;
int total_number_of_iterations;
struct segment_t* segments;
pthread_t* threads;
char* finished_treads;
int number_of_finished_treads;

pthread_mutex_t mutex;
pthread_cond_t conditional;
int number_of_active_threads;
char broadcasted;
char stop_immediately;

struct segment_t {
    int l, r;
};

void* life_computator(void* arg) {
    int i, j, step;
    struct segment_t borders = *((struct segment_t*)arg);
    for (step = 0; step < total_number_of_iterations; step++) {
        for (i = borders.l; i< borders.r; i++) {
            for (j = 0; j < m; j++) {
                recalculate_on_coordinate(field, new_field, n, m, i, j); 
             }
        }
        pthread_mutex_lock(&mutex);
        number_of_active_threads--;
        if (number_of_active_threads > 0) {
            pthread_cond_wait(&conditional, &mutex);
        } else {
            number_of_active_threads = total_number_of_threads;
            two_dimentional_array_swap(&field, &new_field);
            if (stop_immediately) {
                total_number_of_iterations = step;
            }
            pthread_cond_broadcast(&conditional);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void distribute_segments(struct segment_t* segments) {
    int i;
    int k = n / total_number_of_threads;
    int border = n % total_number_of_threads;
    int last = 0;
    for (i = 0; i < total_number_of_threads; i++) {
        segments[i].l = last;
        segments[i].r = last + k;
        if (border) {
            segments[i].r++;
            border--;
        }
        last = segments[i].r + 1;
    }
}

void init_parallel_mode() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conditional, NULL);
    number_of_finished_treads = 0;
    allocate_two_dimentional_array(&new_field, n, m);
    segments = (struct segment_t*)
                    malloc(total_number_of_threads * sizeof(struct segment_t));
    threads = (pthread_t*)malloc(total_number_of_threads * sizeof(pthread_t));
    finished_treads = (char*)calloc(sizeof(char), total_number_of_threads);
}

void run_parallel(int number_of_threads, int number_of_iterations, 
                 char** passed_field, int passed_n, int passed_m) {
    int i;
    if (number_of_threads > n)
        number_of_threads = n;
    n = passed_n;
    m = passed_m;
    field = passed_field;
    total_number_of_threads = number_of_threads;
    total_number_of_iterations = number_of_iterations;
    init_parallel_mode();
    distribute_segments(segments);
    for (i = 0; i < number_of_threads; i++) {
        pthread_create(&threads[i], NULL, life_computator, &segments[i]);
    } 
}

void finalize() {
    if (total_number_of_iterations % 2 == 1) {
        two_dimentional_array_swap(&field, &new_field);
        copy_two_dimentional_array(new_field, field, n, m);
    }
    dispose_two_dimentional_array(new_field, n);
    free(segments);
    free(threads);
    free(finished_treads);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditional);
}

int try_join() {
    int i;
    for (i = 0; i < total_number_of_threads; i++) {
        if (finished_treads[i] == 0 && pthread_tryjoin_np(threads[i], NULL) == 0) {
            finished_treads[i] = 1;
            number_of_finished_treads++;
        }
    }
    if (number_of_finished_treads == total_number_of_threads) {
        finalize();
        return 1;
    } else {
        return 0;
    }
}

void completely_join() {
    int i;
    for (i = 0; i < total_number_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    finalize();
}

void stop() {
    stop_immediately = 1;
    completely_join();
}
