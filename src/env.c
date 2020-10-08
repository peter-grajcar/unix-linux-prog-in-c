#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	if(argc == 1) {
		printf("%s: need at least one argument\n", argv[0]);
		exit(1);
	}

	int i = 1;
	if(argv[i][0] == '-' && !argv[i][1]) {
		//clearenv();
		++i;
	}

	if(argc == 2) {
		printf("%s: need at least two arguments\n", argv[0]);
		exit(1);
	}

	for (; i < argc - 1; ++i) {
		putenv(argv[i]);
	}

	system(argv[i]);
}
