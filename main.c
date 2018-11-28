#include "solver.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int	errno;
#define ESOLVER	-1

int**	mallocGrid()
{
	int **t;
	
	t = (int**)malloc(9 * sizeof(int*));
	if (! t)
	{
		errno = ENOMEM;
		return t;
	}
	for (int i = 0; i < 9; i++)
	{
		t[i] = (int*)malloc(9 * sizeof(int));
		if (! t[i])
		{
			errno = ENOMEM;
			return NULL;
		}
	}
	return t;
}

int loadGrid(int **t, char *file)
{
	FILE	*f;
	int		c;
	
	f = fopen(file, "r");
	if (! f)
	{
		errno = ENOENT;
		return 1;
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			c = fgetc(f);
			if ((c > '0' && c <= '9'))
				t[i][j] = c - '0';
			else if (c == '.')
				t[i][j] = 0;
			else
			{
				fclose(f);
				errno = EINVAL;
				return 2;
			}
		}
		c = fgetc(f);
	}
	fclose(f);
	return 0;
}

void	printGrid(int **t)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (j)
				putchar(' ');
			if (! t[i][j])
				putchar('.');
			else
				putchar('0' + t[i][j]);
		}
		putchar('\n');
	}
}

int _missing(char *bin)
{
	printf("%s: missing file operand\n", bin);
	printf("Try '%s --help' for more information\n", bin);
	return 0;
}

int _help(char *bin)
{
	printf("Usage: %s [FILE]...\n", bin);
	printf("Solve and print sudoku solutions to standard output from FILE(s).\n\n");
	printf("Example data format for FILE:\n");
	printf("..7...2..\n.156.9.4.\n24.1.....\n5.24.1.3.\n16..8..52\n.9.3.21.6\n.....4.83\n.2.7.851.\n..9...7..\n");
	return 0;
}

int	_error(char *bin, char *arg)
{
	if (! errno)
		return 0;
	printf("%s: ", bin);
	if (errno == ESOLVER)
	{
		printf("cannot solve '%s': Forbidden value(s)\n", arg);
		return errno;
	}
	else if (errno == ENOENT || errno == ENOMEM || errno == EINVAL)
		printf("cannot load");
	else
		printf("error");
	printf(" '%s': %s\n", arg, strerror(errno));
	return errno;
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return _missing(argv[0]);
	int **t;
	int i;
	
	for (i = 1; i < argc; i++)
		if (! strcmp(argv[i], "--help"))
			return _help(argv[0]);
	t = mallocGrid();
	if (! t)
		return _error(argv[0], "sudoku");
	for (i = 1; i < argc; i++)
	{
		errno = 0;
		if (! loadGrid(t, argv[i]))
		{
			errno = sudokuSolve(t, 0) + ESOLVER;
			if (! errno)
			{
				if (argc > 2)
				{
					if (i > 1)
						putchar('\n');
					printf("%s:\n", argv[i]);
				}
				printGrid(t);
			}
		}
		if (errno)
			_error(argv[0], argv[i]);
	}
	for (i = 0; i < 9; i++)
		free(t[i]);
	free(t);
	return 0;
}