#include <stdio.h>
#include <poll.h>

int
main(void)
{
	int width = 40;
	int pos = 0;
	int velocity = 1;

	while (1)
	{
		putchar('\r');
		int i;
		
		for (i = 0; i < pos - 1; ++i)
			putchar(' ');
		
		putchar('*');
		
		for (i = pos + 1; i < width; ++i)
			putchar(' ');

		fflush(stdout);

		pos += velocity;
		if (!pos)
			velocity = 1;
		if (pos == width - 1)
			velocity = -1;

		poll(NULL, 0, 100);
	}
}

