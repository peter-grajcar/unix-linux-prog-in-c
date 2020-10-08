#include <stdio.h>
#include <assert.h>
#include "liba.h"
#include "libb.h"

int
main(int argc, char *argv[])
{
	assert(argc > 1);
	printf("liba: %d\n", fna(5, 3));
	printf("libb: %c\n", fnb(argv[1]));
}
