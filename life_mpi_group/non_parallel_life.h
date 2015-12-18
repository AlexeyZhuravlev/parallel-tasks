#ifndef NON_PARALLEL_LIFE
#define NON_PARALLEL_LIFE

int calculate_not_parallel(char** field, int n, int m, int steps_number);
void recalculate_on_coordinate(char** old_field, char** new_field, 
                               int n, int m, int i, int j);


#endif
