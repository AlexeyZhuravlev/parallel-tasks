CC=gcc
FLAGS= -Wall -ansi -pedantic -lncurses -lpthread -O2
life: main.c string_operations.c non_parallel_life.c two_dim_array_operations.c field_generation.c field_display.c parallel_worker.c
	$(CC) $(FLAGS) -o $@ $^
clean:
	rm life
