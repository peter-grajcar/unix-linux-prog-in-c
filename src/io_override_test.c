#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "io_override.h"

int
main(void)
{
	int fd = open("test.txt", O_CREAT, 0660);
	printf("opened file descriptor: %d\n", fd);

	return (0);
}
