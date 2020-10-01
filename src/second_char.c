#include <stdio.h>

int
main(int, char *argv[])
{
	char c = argv[0][1];
	putchar(c > 141 & c < 173 ? c - 32 : c);
	return 0;
}

