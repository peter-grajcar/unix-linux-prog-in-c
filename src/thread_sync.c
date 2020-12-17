#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <poll.h>

#define	N_THREADS	8

int rcpt;
int msg;
int msg_read;
pthread_mutex_t msg_mutex;
pthread_mutex_t msg_read_mutex;
pthread_cond_t msg_read_cond; 

void *
worker(void *data)
{
	int id = (int) data;

	while (1) {
		while (1) {
			pthread_mutex_lock(&msg_mutex);
			if (rcpt != id || msg_read)
				pthread_mutex_unlock(&msg_mutex);
			else
				break;
		}

		printf("Thread %d: %d\n", id, msg);
		msg_read = 1;
		pthread_mutex_unlock(&msg_mutex);
		pthread_cond_signal(&msg_read_cond);
	}

	return (NULL);
}

int
main()
{
	pthread_t threads[N_THREADS];

	pthread_mutex_init(&msg_mutex, NULL);
	pthread_mutex_init(&msg_read_mutex, NULL);
	pthread_cond_init(&msg_read_cond, NULL);

	rcpt = -1;

	for (int i = 0; i < N_THREADS; ++i) {
		pthread_create(&threads[i], NULL, worker, i);
	}

	for (int i = 0; i < 2 * N_THREADS; ++i) {
		pthread_mutex_lock(&msg_mutex);
		
		msg_read = 0;
		msg = rand();
		rcpt = i % N_THREADS;
		printf("sending message %d for thread %d...\n", msg, rcpt);
		poll(NULL, 0, 500);
	
		pthread_mutex_unlock(&msg_mutex);
		while (!msg_read)
			pthread_cond_wait(&msg_read_cond, &msg_read_mutex);
	}

	return (0);
}
