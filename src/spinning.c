#include <stdio.h>
#include <poll.h>

char spinner[] = { '|', '/', '-', '\\' };

int
main(void)
{
	int i = 0;
	while (1) {
		printf("\r%c", spinner[i]);
		fflush(stdout);
		poll(NULL, 0, 500);
		i = (i + 1) % 4;
	}
}
