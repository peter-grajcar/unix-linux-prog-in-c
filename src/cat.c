#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	char buffer[128];
	int i;
	for (i = 1; i < argc; ++i) {
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0)
			err(fd, "open");

		int read_len;
		while ((read_len = read(fd, buffer, sizeof (buffer))) > 0) {
			write(1, buffer, read_len);	
		}

		int close_status = close(fd);
		if (close_status)
			err(close_status, "close");
	}

}
