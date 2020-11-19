#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <err.h>

int
main()
{
	pid_t parent = getpid();
	pid_t child;

	sigset_t signal_set;
	int sig;

	int count = 10;

	switch (child = fork()) {
		case -1:
			errx(1, "fork");
		case 0:
			sigemptyset(&signal_set);
			sigaddset(&signal_set, SIGUSR1);

			while (--count) {
				if (sigwait(&signal_set, &sig))
					errx(1, "sigwait");
				else
					printf("ping\n");

				sleep(1);
				kill(parent, SIGUSR1);
			}

			exit(0);
		default:
			sigemptyset(&signal_set);
			sigaddset(&signal_set, SIGUSR1);

			while (--count) {
				sleep(1);
				kill(child, SIGUSR1);

				if (sigwait(&signal_set, &sig))
					errx(1, "sigwait");
				else
					printf("pong\n");
			}

			exit(0);
	}

	return (0);
}
