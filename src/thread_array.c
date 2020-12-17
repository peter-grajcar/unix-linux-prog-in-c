#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define	ARR_SIZE	64

void *
worker(void *data)
{
	int *arr_el = (int *) data;
	*arr_el = rand();
	return (NULL);
}

int
cmp(const void *a, const void *b)
{
	return (*((int *)a) - *((int *)b));
}

int
main()
{
	int array[ARR_SIZE];
	pthread_t threads[ARR_SIZE];

	memset(array, 0, ARR_SIZE);

	printf("creating threads...");
	for (size_t i = 0; i < ARR_SIZE; ++i) {
		pthread_create(&threads[i], NULL, worker, array + i);
	}
	printf("done.\n");

	printf("waiting for the threads...");
	for (size_t i = 0; i < ARR_SIZE; ++i) {
		pthread_join(threads[i], NULL);
	}
	printf("done.\n");

	qsort(array, ARR_SIZE, sizeof (int), cmp);
	for (size_t i = 0; i < ARR_SIZE; ++i) {
		printf("%d ", array[i]);
	}

	return (0);
}
