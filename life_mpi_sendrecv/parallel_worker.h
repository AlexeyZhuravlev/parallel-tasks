#ifndef PARALLEL_WORKER
#define PARALLEL_WORKER

struct segment_t {
    int l, r;
};

void distribute_segments(struct segment_t* segments, int n, int number_of_slaves);
void send_field(struct segment_t* segments, char** field, int n, int m, 
                int number_of_slaves); 
void receive_field(struct segment_t* segments, char*** field, int n, int m, 
                   int number_of_slaves);
void slave_computations(int rank, int m, int number_of_steps, int number_of_slaves); 

#endif
