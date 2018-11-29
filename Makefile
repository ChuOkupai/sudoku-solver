CC=gcc
CFLAGS=-Wall -Wextra -Werror

clean:
	rm -f *.o *.out

solver.o: solver.c solver.h
	$(CC) $(CFLAGS) -c $< -o $@

solve.out: main.c solver.o
	$(CC) $(CFLAGS) $^ -o $@

run: solve.out
	./$<