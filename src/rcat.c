#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	int i;
	for (i = 1; i < argc; ++i) {
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0)
			errx(fd, "open");

		int offset;
		if ((offset = lseek(fd, -1, SEEK_END)) < 0)
			printf("%d\n", offset);

		int read_len;
		char read_val;
		while ((read_len = read(fd, &read_val, 1)) > 0) {
			printf("%c", read_val);
			if (lseek(fd, -2, SEEK_CUR) < 0)
				break;
		}

		int close_status = close(fd);
		if (close_status)
			err(close_status, "close");
	}

}
