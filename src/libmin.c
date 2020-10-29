#include "libmin.h"
#include <assert.h>

int
min(int a[], size_t len)
{
	assert(len > 0);

	int min = a[0];
	for (size_t i = 1; i < len; ++i)
		if (a[i] < min)
			min = a[i];
	return (min);
}
