/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <errno.h>
#include <err.h>

#define DEFAULT_PATH	"/tmp/mysock"

int main(int argc, const char *const argv[])
{
	socklen_t alen = sizeof(struct sockaddr_un);
	const char *path = DEFAULT_PATH;
	struct sockaddr_un addr;
	int ret, fd;

	if (argc > 1)
		path = argv[1];

	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fd == -1)
		err(EXIT_FAILURE, "socket(%s) error", path);

	ret = remove(path);
	if (ret == -1 && errno != ENOENT)
		err(EXIT_FAILURE, "remove(%s) error", path);

	/* local address */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(&addr.sun_path[1], path, sizeof(addr.sun_path)-2);
	ret = bind(fd, (struct sockaddr *)&addr, alen);
	if (ret == -1)
		err(EXIT_FAILURE, "bind(%s) error", path);

	for (;;) {
		char *p, buf[BUFSIZ];
		ssize_t n, remain;
		int i;

		alen = sizeof(struct sockaddr_un);
		n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&addr,
			     &alen);
		if (n == -1)
			err(EXIT_FAILURE, "recvfrom error");
		else if (n == 0)
			err(EXIT_FAILURE, "socket closed");

		/* to upper case */
		for (i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);

		/* send it back */
		for (p = buf, remain = n; remain > 0; p += n, remain -= n) {
			n = sendto(fd, p, remain, 0, (struct sockaddr *)&addr,
				   alen);
			if (n == -1)
				err(EXIT_FAILURE, "sendto error");
		}
	}
	ret = close(fd);
	if (ret == -1)
		err(EXIT_FAILURE, "close(%d) error", fd);

	exit(EXIT_SUCCESS);
}
