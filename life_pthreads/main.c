#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <time.h>
#include "string_operations.h"
#include "non_parallel_life.h"
#include "two_dim_array_operations.h"
#include "field_generation.h"
#include "field_display.h"
#include "parallel_worker.h"

char** field = NULL;
int n, m, k;
char running = 0;

int interpret_command(char** command_array, int command_array_length) {
    if (strcmp(command_array[0], "START") == 0) {
       if (command_array_length < 2) {
           printf("Please enter initial distribution and number of threads\n");
       } else if (running) {
           printf("Unable to init new filed. Stop calculations first\n");
       } else if (strcmp(command_array[1], "random") == 0) {
           if (command_array_length != 5) {
               printf("For random initial distribution please type three numbers: sizes of field, and number of threads\n");
           } else {
               if (field != NULL) {
                   dispose_two_dimentional_array(field, n);
               }
               n = atoi(command_array[2]);
               m = atoi(command_array[3]);
               k = atoi(command_array[4]);
               generate_field_at_random(n, m, &field);
           }
       } else if (strcmp(command_array[1], "file") == 0) {
          if (command_array_length != 4) {
              printf("For initial distribution from file please type name of file: and number of threads\n");
          } else {
              FILE* file;
              if (field != NULL) {
                  dispose_two_dimentional_array(field, n);
              }
              file = fopen(command_array[2], "r");
              if (read_field_from_file(file, &field, &n, &m)) {
                  printf("Invalid input file format. It should be correct CSV 0-1 table\n");
              }
              fclose(file);
              k = atoi(command_array[3]);
          }
       } else {
           printf("Invalid option for start. Type \"random\" for random distribution or \"file\" for distribution from file.\n");
       }
       return 0;
    } else if (strcmp(command_array[0], "STATUS") == 0) {
        if (running) {
            printf("Unable to display status while running. Type STOP to stop calculations\n");
        } else {
            display_field(field, n, m);
        }
        return 0;
    } else if (strcmp(command_array[0], "RUN") == 0) {
        if (command_array_length < 2) {
            printf("Enter number of steps to run\n");
        } else if (field == NULL) {
            printf("Load field to run. Use START command\n");
        } else {
            int steps_number = atoi(command_array[1]);
            run_parallel(k, steps_number, field, n, m);
            running = 1;
        }
        return 0;
   } else if (strcmp(command_array[0], "STOP") == 0) {
       if (running == 0) {
           printf("Not running anything to stop\n"); 
       } else {
           int iteration_number = stop();
           printf("Calculatons were stopped on %d iteration\n", iteration_number);
           running = 0;
       }
       return 0;
    } else if (strcmp(command_array[0], "QUIT") == 0) {
        if (running) {
            stop();
            running = 0;
        }
        if (field != NULL) {
            dispose_two_dimentional_array(field, n);
        }
        return 1;
    } else if (strcmp(command_array[0], "MEASURE") == 0) {
        if (command_array_length < 5) {
            printf("Type: sizes of field, number of threads, number of steps.\n");
        } else {
            if (field != NULL) {
                dispose_two_dimentional_array(field, n);
            } else {
                int steps;
                clock_t time;
                double not_parallel_time, parallel_time;
                n = atoi(command_array[1]);
                m = atoi(command_array[2]);
                k = atoi(command_array[3]);
                steps = atoi(command_array[4]);
                generate_field_at_random(n, m, &field);
                time = clock();
                calculate_not_parallel(field, n, m, steps); 
                not_parallel_time = difftime(clock(), time) / CLOCKS_PER_SEC;
                time = clock();
                run_parallel(k, steps, field, n, m);
                parallel_time = difftime(clock(), time) / CLOCKS_PER_SEC;
                completely_join(); 
                printf("Not parallel: %f sec\n", not_parallel_time);
                printf("Parallel: %f sec\n", parallel_time);
                printf("Acceleration: %f\n", not_parallel_time / parallel_time);
                dispose_two_dimentional_array(field, n);
                field = NULL;
            }
        }
        return 0;
    } else {
        printf("Unknown command: \"%s\"\n", command_array[0]);
        return 0;
    }
}

int main(int argc, char** argv) {
    char* command;
    char** command_array;
    int command_array_length;
    int exit_flag = 0;
    srand(time(NULL));
    printf("LIFE$ ");
    while (exit_flag == 0 && safe_gets(stdin, &command) != EOF) { 
        if (running && try_join()) {
            running = 0;
            printf("\nCalculations were finished\n");
        }
        if (strcmp(command, "") != 0) {
            parse_by_delimiter(command, ' ', &command_array, &command_array_length);
            if (interpret_command(command_array, command_array_length)) {
                exit_flag = 1;
            }
            dispose_two_dimentional_array(command_array, command_array_length);
        }
        free(command);
        if (!exit_flag) {
            printf("LIFE$ ");
        }
    }
    _nc_freeall();
    return 0;
}
