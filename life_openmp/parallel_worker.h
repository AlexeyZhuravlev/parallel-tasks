#ifndef PARALLEL_WORKER
#define PARALLEL_WORKER

void run_parallel(int number_of_threads, int number_of_iterations, char** passed_field, int passed_n, int passed_m);
int stop();
char is_finished();

#endif
