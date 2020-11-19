#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int
main()
{
	sem_t *ping = sem_open("ping", O_CREAT | O_EXCL, S_IRWXU, 1);
	sem_t *pong = sem_open("pong", O_CREAT | O_EXCL, S_IRWXU, 0);

	int count = 10;

	pid_t child;
	switch (child = fork()) {
		case -1:
			errx(1, "fork");
		case 0:
			while (--count) {
				sem_wait(ping);
				printf("ping\n");
				sem_post(pong);
			}
			exit(0);
		default:
			while (--count) {
				sem_wait(pong);
				printf("pong\n");
				sem_post(ping);
			}
	}

	int status;
	wait(&status);

	sem_unlink("ping");
	sem_unlink("pong");
	
	return (0);
}
