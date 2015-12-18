CC=mpicc
FLAGS=-Wall -ansi -O3 
life: main.c string_operations.c non_parallel_life.c two_dim_array_operations.c field_generation.c parallel_worker.c
	$(CC) $(FLAGS) -o $@ $^
clean:
	rm life
