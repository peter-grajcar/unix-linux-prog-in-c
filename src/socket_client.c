#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define	BUFFER_SIZE	100

int
main(int argc, char **argv)
{
	int fd;
	int status;
	struct sockaddr_in6 server_addr;

	if (argc != 3)
		errx(1, "usage: socket_client addr port");

	fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1)
		errx(1, "socket()");

	//int disable = 0;
	//status = setsockopt(fd, SOL_SOCKET, IPV6_V6ONLY, &disable, sizeof (int));
	//if (status == -1)
	//	errx(1, "setsockopt() = %d", errno);

	server_addr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, argv[1], &server_addr.sin6_addr);
	int port = atoi(argv[2]);
	server_addr.sin6_port = htons(port);

	status = connect(fd, (struct sockaddr *)&server_addr, sizeof (server_addr));
	if (status == -1)
		errx(1, "connect()");

	status = write(fd, "Hello, World!", 14);
	if (status == -1)
		errx(1, "write()");

	status = close(fd);
	if (status == -1)
		errx(1, "close()");

	return (0);
}
