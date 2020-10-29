#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <utmpx.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>

#define	USAGE	"usage: fake_who [-h] [-u username] [-a hostname] [-f file] [pattern ...]"
#define	HELP	"\tAlters utmpx records which match the patterns. The patterns have\n"\
				"\tfolowing format: 'x:value' where x can be 'u' or 'h' (see options).\n"\
				"\tAll patterns have to be mathced to change the record.\n\n"\
				"\tThe options are as follows:\n\n"\
				"\t-h\n\t\tprints basic help.\n"\
				"\t-u username\n\t\tsets new username on match.\n"\
				"\t-a hostname\n\t\tsets new hostname on match.\n"\
				"\t-f file\n\t\tspecifies path to utmpx file. default is /var/run/utmpx.\n"

int
main(int argc, char *argv[])
{
	char *path = "/var/run/utmpx";
	char *username = NULL;
	char *hostname = NULL;

	int opt;
	while ((opt = getopt(argc, argv, "hu:a:f:")) != -1) {
		switch (opt) {
			case 'u':
				username = optarg;
				break;
			case 'a':
				hostname = optarg;
				break;
			case 'f':
				path = optarg;
				break;
			case 'h':
				printf("%s\n\n%s", USAGE, HELP);
				return (0);
			default:
				return (1);
		}
	}

	// nothing to be done
	if (!username && !hostname)
		return (0);

	char *match_username = NULL;
	char *match_hostname = NULL;
	for (int i = optind; i < argc; ++i) {
		if (argv[i][0] && argv[i][1] != ':')
			errx(1, "invalid pattern '%s'", argv[i]);
		switch (argv[i][0]) {
			case 'a':
				match_hostname = argv[i] + 2;
				break;
			case 'u':
				match_username = argv[i] + 2;
				break;
			default:
				errx(1, "unknown field '%c' in pattern '%s'", argv[i][0], argv[i]);
		}
	}

	struct stat info;
	int stat_status = stat(path, &info);
	if (stat_status)
		errx(stat_status, "stat");

	// save original the modification time
	struct utimbuf orig_time = {
		.actime = info.st_atime,
		.modtime = info.st_mtime
	};

	// open utmpx file
	int fd = open(path, O_RDWR);
	if (fd < 0)
		errx(fd, "open");

	// magic constant 1256, offset from the beginning of the utmpx file
	int seek_off = lseek(fd, 1256, SEEK_SET);
	if (seek_off < 0)
		errx(seek_off, "lseek");

	struct utmpx record;
	size_t read_size;
	size_t len = sizeof (struct utmpx) - 12;
	off_t off = (off_t) len;

	// read the records
	while ((read_size = read(fd, &record, len)) == len) {
		if (match_username && strncmp(match_username, record.ut_host, _UTX_USERSIZE))
			continue;
		if (match_hostname && strncmp(match_hostname, record.ut_user, _UTX_HOSTSIZE))
			continue;

		if (username)
			strncpy(record.ut_user, username, _UTX_USERSIZE);
		if (hostname)
			strncpy(record.ut_host, hostname, _UTX_HOSTSIZE);

		seek_off = lseek(fd, -off, SEEK_CUR);
		if (seek_off > 0)
			write(fd, &record, len);
	}

	int close_status = close(fd);
	if (close_status)
		errx(fd, "close");

	// restore the original modification time
	int utime_status = utime(path, &orig_time);
	if (utime_status)
		errx(utime_status, "utime");

	return (0);
}
