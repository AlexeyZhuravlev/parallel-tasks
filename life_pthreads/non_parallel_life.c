#include <stdlib.h>
#include "two_dim_array_operations.h"


int number_of_neighbours(char** field, int n, int m, int i, int j) {
    int k, sum, neighbour_i, neighbour_j;
    int di[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
    int dj[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
    sum = 0;
    for (k = 0; k < 8; k++) {
        neighbour_i = normalize_koordinate(i + di[k], n);
        neighbour_j = normalize_koordinate(j + dj[k], m);
        sum += (int)field[neighbour_i][neighbour_j];
    }
    return sum;
}

void recalculate_on_coordinate(char** old_field, char** new_field, 
                               int n, int m, int i, int j) {
    int k;
    k = number_of_neighbours(old_field, n, m, i, j);
    if (k == 2) {
        new_field[i][j] = old_field[i][j];
    } else if (k == 3) {
        new_field[i][j] = 1; 
    } else {
        new_field[i][j] = 0;
    } 
}

void recalculate_position(char** old_field, char** new_field, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            recalculate_on_coordinate(old_field, new_field, n, m, i, j);
        }
    }
}

void calculate_not_parallel(char** field, int n, int m, int steps_number) {
    int step;
    char** new_field;
    allocate_two_dimentional_array(&new_field, n, m);
    for (step = 0; step < steps_number; step++) {
       recalculate_position(field, new_field, n, m);
       copy_two_dimentional_array(new_field, field, n, m);
    }
    dispose_two_dimentional_array(new_field, n);
}
