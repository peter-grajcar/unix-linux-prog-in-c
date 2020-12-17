#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>
#include <poll.h>

pthread_mutex_t about_to_return_mutex;
pthread_mutex_t can_return_mutex;
pthread_cond_t about_to_return;

pthread_mutex_t worker_id_mutex;
int next_worker_id = 0;

int
issue_worker_id()
{
	pthread_mutex_lock(&worker_id_mutex);
	int id = next_worker_id++;
	pthread_mutex_unlock(&worker_id_mutex);
	return (id);
}

void *
worker(void *data)
{
	int worker_id = issue_worker_id();
	int timeout = 1000 + rand() % 5000;
	poll(NULL, 0, timeout);
	printf("worker #%d done\n", worker_id);

	pthread_mutex_lock(&can_return_mutex);
	pthread_cond_signal(&about_to_return);
	pthread_mutex_unlock(&can_return_mutex);

	return (NULL);
}

pthread_t *
add_thread_worker()
{
	pthread_t *thread = malloc(sizeof (pthread_t));
	if (!thread)
		errx(1, "could not allocate memory for the thread pool");
	pthread_create(thread, NULL, worker, NULL);
	return (thread);
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		errx(1, "usage: %s <number_of_threads>", argv[0]);
	}

	int thread_count = atoi(argv[1]);

	pthread_mutex_init(&worker_id_mutex, NULL);
	pthread_mutex_init(&about_to_return_mutex, NULL);
	pthread_cond_init(&about_to_return, NULL);
	pthread_mutex_init(&can_return_mutex, NULL);

	for (int i = 0; i < thread_count; ++i) {
		add_thread_worker();
	}


	while (1) {
		pthread_cond_wait(&about_to_return, &about_to_return_mutex);
		pthread_mutex_lock(&can_return_mutex);
		add_thread_worker();
		pthread_mutex_unlock(&can_return_mutex);
	}

	return (0);
}
