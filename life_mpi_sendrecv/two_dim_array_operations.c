#include <stdlib.h>

void two_dimentional_array_swap(char*** a, char*** b) {
    char** c = *a;
    *a = *b;
    *b = c;
}

int normalize_koordinate(int koordinate, int limit) {
    if (koordinate < 0)
        return limit + koordinate;
    return koordinate % limit;
}

void allocate_two_dimentional_array(char*** array, int n, int m) {
    int i;
    *array = (char**)malloc(n * sizeof(char*));
    for (i = 0; i < n; i++) {
        (*array)[i] = (char*)malloc(m * sizeof(char));
    }
}

void dispose_two_dimentional_array(char** array, int size) {
    int i;
    for (i = 0; i < size; i++)
        free(array[i]);
    free(array);
}

void copy_two_dimentional_array(char** from, char** to, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            to[i][j] = from[i][j];
        }
    }
}

void translate_matrix_to_vector(char** matrix, char** vector, int n, int m) {
    int i;
    *vector = (char*)malloc(n * m * sizeof(char));
    for (i = 0; i < n * m; i++) {
        (*vector)[i] = matrix[i / m][i % m];
    }
}

void translate_vector_to_matrix(char* vector, char** matrix, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            matrix[i][j] = vector[i * m + j];
        }
    }
}
