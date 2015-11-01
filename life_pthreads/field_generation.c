#include "string_operations.h"
#include "two_dim_array_operations.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int read_field_from_file(FILE* file, char*** field, int* n, int* m) {
    char* str;
    char** result;
    char* current_line;
    int size, i;
    char errorflag = 0;
    *m = 0;
    *n = 0;
    *field = NULL;
    while (errorflag == 0 && safe_gets(file, &str) != EOF) {
        parse_by_delimiter(str, ',', &result, &size);
        if ((*m) != 0) {
            *m = size;
        } else if ((*m) != size) {
            errorflag = 1;
        }
        current_line = (char*)malloc(size * sizeof(char));
        for (i = 0; i < size; i++) {
            if (strcmp(result[i], "1") == 0 || strcmp(result[i], "0")) {
                current_line[i] = (result[i][0] - '0');
            } else {
                errorflag = 1;
            }
        }
        add_string_to_list(field, current_line, n);
        dispose_two_dimentional_array(result, size);
        free(str);
    }
    if (errorflag) {
        dispose_two_dimentional_array(*field, *n);
        *field = NULL;
        return 1;
    } else {
        return 0;
    }
}

void generate_field_at_random(int n, int m, char*** field) {
    int i, j;
    allocate_two_dimentional_array(field, n, m);
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            (*field)[i][j] = (char)(rand() % 2);
        }
    }
}
