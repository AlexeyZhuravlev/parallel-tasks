#include <stdlib.h>

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

