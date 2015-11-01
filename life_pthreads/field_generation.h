#ifndef FIELD_GENERATION
#define FIELD_GENERATION

int read_field_from_file(FILE* file, char*** field, int* n, int* m);
void generate_field_at_random(int n, int m, char*** field);

#endif
