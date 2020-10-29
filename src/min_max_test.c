#include "libmin.h"
#include "libmax.h"
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	int len = argc - 1;
	int *a = malloc(sizeof (int) * len);

	for (int i = 0; i < len; ++i)
		a[i] = atoi(argv[i + 1]);

	printf("min: %d\nmax: %d\n", min(a, len), max(a, len));

	free(a);

	return (0);
}
