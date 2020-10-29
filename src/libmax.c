#include "libmax.h"
#include <assert.h>

int
max(int a[], size_t len)
{
	assert(len > 0);

	int max = a[0];
	for (size_t i = 1; i < len; ++i)
		if (a[i] > max)
			max = a[i];
	return (max);
}
