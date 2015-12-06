#ifndef TWOD_ARRAY_OPERATIONS
#define TWOD_ARRAY_OPERATIONS

void allocate_two_dimentional_array(char*** array, int n, int m);
void dispose_two_dimentional_array(char** array, int size);
void copy_two_dimentional_array(char** from, char** to, int n, int m);
int normalize_koordinate(int koordinate, int limit);
void two_dimentional_array_swap(char*** first, char*** second);
void translate_matrix_to_vector(char** matrix, char** vector, int n, int m);
void translate_vector_to_matrix(char* vector, char** matrix, int n, int m);


#endif
