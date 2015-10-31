#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_operations.h"
#include "non_parallel_life.h"

int interpret_command(char** command_array, int command_array_length) {
    if (strcmp(command_array[0], "START") == 0) {
        
    } else if (strcmp(command_array[0], "STATUS") == 0) {

    } else if (strcmp(command_array[0], "RUN") == 0) {
        
    } else if (strcmp(command_array[0], "STOP") == 0) {
       printf("Not running anything to stop"); 
    } else if (strcmp(command_array[0], "QUIT") == 0) {
       return 1; 
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
    while (exit_flag == 0 && safe_gets(stdin, &command) != EOF) { 
        parse_by_delimiter(command, ' ', &command_array, &command_array_length);
        if (interpret_command(command_array, command_array_length)) {
            exit_flag = 1;
        }
        free(command);
        free_words_array(command_array, command_array_length);
    }
    return 0;
}
