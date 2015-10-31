#ifndef STRING_OPERATIONS
#define STRING_OPERATIONS

int safe_gets(FILE *f, char** string);
void parse_by_delimiter(char* string, char delimter, char*** result, 
                        int* result_size);
void free_words_array(char** array, int size);


#endif
