#ifndef PARALLEL_WORKER
#define PARALLEL_WORKER

struct segment_t {
    int l, r;
};
void scatter_field(char** field, int n, int m, int number_of_nodes, int rank);
void calculate_in_parallel(int rank, int number_of_iterations, 
                           int number_of_nodes, int m); 
void gather_field(char** field, int n, int m, int number_of_nodes, int rank); 

#endif
