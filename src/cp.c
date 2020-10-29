#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
	if (argc != 3)
		errx(1, "usage: cp src_file dst_file");

	char buffer[128];

	int src = open(argv[1], O_RDONLY);
	if (src < 0)
		errx(src, "open (src)");

	struct stat src_stat;
	int stat_status;
	if((stat_status = stat(argv[1], &src_stat)) < 0)
		errx(stat_status, "stat");

	int dst = open(argv[2], O_WRONLY | O_CREAT, src_stat.st_mode);
	if (dst < 0)
		errx(dst, "open (dst)");

	int read_len;
	while ((read_len = read(src, buffer, sizeof (buffer))) > 0) {
		write(dst, buffer, read_len);
	}

	int close_status;
	if ((close_status = close(src)) != 0)
		err(close_status, "close");
	if ((close_status = close(dst)) != 0)
		err(close_status, "close");

}
