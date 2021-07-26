/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <err.h>

#define DEFAULT_PATH	"/tmp/mysock"

int main(int argc, const char *const argv[])
{
	socklen_t alen = sizeof(struct sockaddr_un);
	const char *path = DEFAULT_PATH;
	struct sockaddr_un addr;
	int fd, ret, pid;

	if (argc > 1)
		path = argv[1];

	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fd == -1)
		err(EXIT_FAILURE, "socket(%s) error", path);

	/* bind to receive a message */
	pid = getpid();
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	ret = snprintf(&addr.sun_path[1], sizeof(addr.sun_path)-2,
		       "%s.%ld", path, (long)pid);
	if (ret < 0)
		err(EXIT_FAILURE, "snprintf error");
	ret = bind(fd, (struct sockaddr *)&addr, alen);
	if (ret == -1)
		err(EXIT_FAILURE, "bind(%s) error", addr.sun_path);

	/* server address */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(&addr.sun_path[1], path, sizeof(addr.sun_path)-2);

	/* read msg from stdin and write the server responce to stdout. */
	for (;;) {
		char *p, buf[BUFSIZ];
		ssize_t n, remain;

		/* rpc */
		while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
			while (buf[n-1] == '\n')
				n--;
			if (n == 0)
				continue;
			for (p = buf, remain = n; remain > 0; p += n, remain -= n)
				if ((n = sendto(fd, p, remain, 0,
						(struct sockaddr *)&addr, alen)) == -1)
					err(EXIT_FAILURE, "sendto error");

			/* receive */
			n = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
			if (n == -1)
				err(EXIT_FAILURE, "recvfrom error");
			else if (n == 0) {
				fprintf(stderr, "server closed the connection");
				break;
			}
			for (p = buf, remain = n; remain > 0; p += n, remain -= n)
				if ((n = write(STDOUT_FILENO, p, remain)) == -1)
					err(EXIT_FAILURE, "write(stdout) error");
			printf("\n");
		}
		if (n == -1)
			err(EXIT_FAILURE, "read error");
		else if (n == 0) {
			printf("stdin or socket has been closed, exiting...\n");
			break;
		}
	}
	ret = close(fd);
	if (ret == -1)
		err(EXIT_FAILURE, "close error");

	exit(EXIT_SUCCESS);
}
