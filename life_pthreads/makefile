CC=gcc
FLAGS= -Wall -ansi -pedantic -g
life: main.c string_operations.c non_parallel_life.c
	$(CC) $(FLAGS) -o $@ $^
clean:
	rm life
