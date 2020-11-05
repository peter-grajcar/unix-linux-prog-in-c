#include <stdio.h>
#include <unistd.h>
#include <err.h>

int
parent(int pipe[])
{
	close(0);
	int fd = dup(pipe[0]);

	if (fd < 0)
		errx(1, "dup (parent)");

	close(pipe[0]);
	close(pipe[1]);

	return (execlp("wc", "wc", "-l", NULL));
}

int
child(int pipe[])
{
	close(1);
	int fd = dup(pipe[1]);

	if (fd < 0)
		errx(1, "dup (child)");

	close(pipe[0]);
	close(pipe[1]);

	return (execlp("ls", "ls", "-l", "/", NULL));
}

int
main(void)
{
	int fildes[2];

	if (pipe(fildes) < 0)
		errx(1, "pipe");

	printf("ls -l / | wc -l\n");

	int pid;
	switch (pid = fork()) {
		case -1:
			errx(1, "fork");
		case 0:
			return (child(fildes));
		default:
			return (parent(fildes));
	}
}
