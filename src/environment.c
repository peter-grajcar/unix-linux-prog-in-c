#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int
main()
{
	int i = 0;
	while (environ[i] != NULL)
		printf("%s\n", environ[i++]);

	printf("---------------------\n");

	printf("HOME: %s\n", getenv("HOME"));
}
