#include "libmax.h"
#include "libmin.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define	HIST_SIZE	50

int
main(int argc, char *argv[])
{
	assert(argc > 1);
	int len = argc - 1;
	int a[len];
	
	for (int i = 0; i < len; ++i) 
		a[i] = atoi(argv[i + 1]);

	int hist[HIST_SIZE] = { 0, };

	int x0 = min(a, len);
	int x1 = max(a, len);
	assert(x1 - x0 > HIST_SIZE);
	int step = 1 + (x1 - x0) / (HIST_SIZE - 1);

	for (int i = 0; i < len; ++i) {
		int idx = (a[i] - x0) / step;
		++hist[idx];
	}

	int rows = max(hist, HIST_SIZE);

	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < HIST_SIZE; ++i)
			printf("%c ",  hist[i] >= rows - j ? '#' : ' ');
		putchar('\n');
	}

	return (0);
}
