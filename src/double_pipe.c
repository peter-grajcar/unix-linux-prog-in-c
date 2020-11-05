#include <unistd.h>
#include <stdio.h>
#include <err.h>

int
child1(int pipes[], int pipe_count)
{
	close(1);
	dup(pipes[1]);

	for (int i = 0; i < pipe_count; ++i)
		close(pipes[i]);

	return (execlp("cal", "cal", NULL));
}

int
child2(int pipes[], int pipe_count)
{
	int *pipe_in = pipes;
	int *pipe_out = pipes + 2;

	close(0);
	close(1);

	dup(pipe_in[0]);
	dup(pipe_out[1]);

	for (int i = 0; i < pipe_count; ++i)
		close(pipes[i]);

	return (execlp("head", "head", "-1", NULL));
}

int
parent(int pipes[], int pipe_count)
{
	close(0);
	dup(pipes[2]);

	for (int i = 0; i < pipe_count; ++i)
		close(pipes[i]);

	return (execlp("tr", "tr", "'[[:lower:]]'", "'[[:upper:]]'", NULL));
}

int
main(void)
{
	int pipes[4];
	int pipe_status;

	if ((pipe_status = pipe(pipes)) < 0)
		errx(1, "pipe");
	if ((pipe_status = pipe(pipes + 2)) < 0)
		errx(1, "pipe");

	pid_t pid;

	pid = fork();
	if (pid == 0)
		return (child1(pipes, 4));

	pid = fork();
	if (pid == 0)
		return (child2(pipes, 4));

	return (parent(pipes, 4));
}
