#include <stdio.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <err.h>

static int (*real_open)(const char *path, int oflag, ...);

int
open(const char *path, int oflag, ...)
{
	va_list args;
	va_start(args, oflag);
	printf("opening file %s...\n", path);

	if (real_open == NULL) {
		if ((real_open = dlsym(RTLD_NEXT, "open")) == NULL)
			errx(1, "not found: open");
	}

	int mode = 0;
	if (oflag & O_CREAT)
		mode = va_arg(args, int);

	va_end(args);
	return (real_open(path, oflag, mode));
}
