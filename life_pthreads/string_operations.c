#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define REALLOC_PROBLEM 1;
#define READING_PROBLEM 2;

/* You should free memory before reading another string to the same char* variable
 * Example
 * char* s = safe_gets(stdin);
 * _someoperations_
 * free(s);
 * s = safe_gets(stdin);
 * _new_operations_
 * free(s);         
 */

int safe_gets(FILE *f, char** string)
{
    int capacity = 0;
    int length = 0;
    char* result = NULL;
    char* success;
    char new_symbol;
    int pagesize = sysconf(_SC_PAGESIZE);
    if (f == NULL)
        return READING_PROBLEM;
    do
    {
        new_symbol = fgetc(f);
        if (new_symbol == EOF)
        {
            if (ferror(f))
            {
                free(result);
                return READING_PROBLEM;
            }
            else if (feof(f))
            {
                if (length == 0)
                    return EOF;
                else
                    new_symbol = '\0';
            }
        }
        if (new_symbol == '\n')
            new_symbol = '\0';

        length++;
        if (length > capacity)
        {
            capacity += pagesize;
            success = (char*)realloc(result, capacity * sizeof(char));
            if (success == NULL)
            {
                free(result);
                return REALLOC_PROBLEM;
            }
            result = success;
        }
        result[length - 1] = new_symbol;
    }
    while (new_symbol != '\0');
    success = (char*)realloc(result, length * sizeof(char));
    if (success == NULL)
    {
        free(result);
        return REALLOC_PROBLEM;
    }
    result = success;
    *string = result;
    return 0;
}

void add_symbol_to_string(char** current_string, char symbol, int* current_length) {
    (*current_length)++;
    *current_string = realloc(*current_string, (*current_length) * sizeof(char));
    (*current_string)[(*current_length) - 1] = symbol;
}

void add_string_to_list(char*** list, char* string, int* length) {
    (*length)++;
    *list = realloc(*list, (*length) * sizeof(char*));
    (*list)[(*length) - 1] = string;
}

/* One who uses parse_by_delimiter should free the allocated by function memory
 * with the call of function free_words_array before second usage with same arguments
 * Example:
 * char** array;
 * int length;
 * parse_by_delimter(str, ' ', &array, &length);
 * ...
 * free_words_array(array, length);
 */

void parse_by_delimiter(char* string, char delimiter, 
                        char*** result, int* result_size) {
    char* current_string;
    int current_length, i;
    current_length = 0;
    current_string = NULL;
    *result = NULL;
    *result_size = 0; 
    for (i = 0; string[i] != '\0'; i++) {
        if (string[i] == delimiter) {
            add_symbol_to_string(&current_string, '\0', &current_length);
            add_string_to_list(result, current_string, result_size);
            current_length = 0;
            current_string = NULL;
        } else {
            add_symbol_to_string(&current_string, string[i], &current_length);
        }
    }           
    add_symbol_to_string(&current_string, '\0', &current_length);
    add_string_to_list(result, current_string, result_size); 
}

void free_words_array(char** array, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}
