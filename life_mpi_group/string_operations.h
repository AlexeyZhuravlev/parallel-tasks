#ifndef STRING_OPERATIONS
#define STRING_OPERATIONS
#include <stdio.h>

int safe_gets(FILE *f, char** string);
void parse_by_delimiter(char* string, char delimter, char*** result, 
                        int* result_size);
void add_string_to_list(char*** list, char* string, int* length);


#endif
