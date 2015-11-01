#ifndef TWOD_ARRAY_OPERATIONS
#define TWOD_ARRAY_OPERATIONS

void allocate_two_dimentional_array(char*** array, int n, int m);
void dispose_two_dimentional_array(char** array, int size);
void copy_two_dimentional_array(char** from, char** to, int n, int m);
int normalize_koordinate(int koordinate, int limit);


#endif
