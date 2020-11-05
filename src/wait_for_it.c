#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>

int
parent(pid_t child_pid)
{
	printf("Child process PID: %d\n", child_pid);

	int status;
	do {
		pid_t wait_pid = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);

		printf("PID: %d SIGNAL: %d\n", wait_pid, WTERMSIG(status));
	} while (!WIFSIGNALED(status) && !WIFEXITED(status));

	return (0);
}

int
child(void)
{
	return (execlp("sleep", "sleep", "30", NULL));
}

int
main(void)
{
	pid_t pid;
	switch (pid = fork()) {
		case -1:
			errx(1, "fork");
		case 0:
			return (child());
		default:
			return (parent(pid));
	}
}
