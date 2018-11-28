CC=gcc
CFLAGS=-Wall -Wextra -Werror

clean:
	rm -f solve

solve.out: *.h *.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@