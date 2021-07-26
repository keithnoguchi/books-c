/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <err.h>

#define DEFAULT_PATH	"/tmp/mysock"

int main(int argc, const char *const argv[])
{
	const char *path = DEFAULT_PATH;
	struct sockaddr_un addr;
	socklen_t alen;
	int fd, ret;

	if (argc > 1)
		path = argv[1];

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1)
		err(EXIT_FAILURE, "socket error");

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
	alen = sizeof(addr);
	ret = connect(fd, (struct sockaddr *)&addr, alen);
	if (ret == -1)
		err(EXIT_FAILURE, "connect(%s) error", path);

	while (1) {
		char buf[BUFSIZ], *p;
		ssize_t ret, out;

		ret = read(STDIN_FILENO, buf, sizeof(buf));
		if (ret == -1)
			err(EXIT_FAILURE, "read(stdin) error");
		else if (ret == 0)
			break;

		/* write out the message over the socket */
		for (p = buf, out = ret; out > 0; p += out, out -= ret) {
			if ((ret = write(fd, p, out)) == -1)
				err(EXIT_FAILURE, "write(%d) error", fd);
		}
	}
	ret = close(fd);
	if (ret == -1)
		err(EXIT_FAILURE, "close error");

	exit(EXIT_SUCCESS);
}
