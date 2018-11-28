#include "solver.h"

int	getSquare(int n)
{
	return n - n % 3;
}

int	isValid(int **t, int l, int c, int n)
{
	int i;
	
	for (i = 0; i < 9; i++)
		if (t[i][c] == n || t[l][i] == n)
			return 0;
	for (i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (t[getSquare(l) + i][getSquare(c) + j] == n)
				return 0;
	return 1;
}

int	sudokuSolve(int **t, int n)
{
	if (n > 80)
		return 1;
	int i;
	int j;

	i = n / 9;
	j = n % 9;
	if (t[i][j])
		return sudokuSolve(t, n + 1);
	for (int k = 1; k < 10; k++)
	{
		if (! isValid(t, i, j, k))
			continue;
		t[i][j] = k;
		if (sudokuSolve(t, n + 1))
			return 1;
		else
			t[i][j] = 0;
	}
	return 0;
}