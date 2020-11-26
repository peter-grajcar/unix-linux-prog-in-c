#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <err.h>

#define	CLIENT_QUEUE_LEN	10
#define	SERVER_PORT			10088
#define	BUFFER_SIZE			100

int
main(void)
{
	char buf[BUFFER_SIZE];

	int server_fd = -1, client_fd = -1;
	struct sockaddr_in6 server_addr, client_addr;
	socklen_t client_addr_len;
	char str_addr[INET6_ADDRSTRLEN];
	int status;

	server_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd == -1)
		errx(1, "socket()");

	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_any;
	server_addr.sin6_port = htons(SERVER_PORT);

	status = bind(server_fd, (struct sockaddr *)&server_addr, sizeof (server_addr));
	if (status == -1)
		errx(1, "bind()");

	status = listen(server_fd, CLIENT_QUEUE_LEN);
	if (status == -1)
		errx(1, "listen()");

	client_addr_len = sizeof (client_addr);

	while (1) {
		client_fd = accept(server_fd,
				(struct sockaddr *)&client_addr,
				&client_addr_len);

		if (client_fd == -1)
			errx(1, "accept()");

		inet_ntop(AF_INET6, &(client_addr.sin6_addr),
				str_addr, sizeof (str_addr));
		printf("New connection from: %s:%d ...\n",
				str_addr,
				ntohs(client_addr.sin6_port));

		status = read(client_fd, buf, BUFFER_SIZE);
		if (status == -1) {
			err(1, "read()");
			close(client_fd);
			continue;
		}

		printf("%s\n", buf);

		status = close(client_fd);
		if (status == -1)
			errx(1, "close()");

		printf("Connection closed\n");
	}

	return (0);
}
