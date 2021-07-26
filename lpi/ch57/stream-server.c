/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <err.h>
#include <errno.h>

#define DEFAULT_PATH	"/tmp/mysock"
#define DEFAULT_BACKLOG	5

int main(int argc, const char *const argv[])
{
	socklen_t alen = sizeof(struct sockaddr_un);
	const char *path = DEFAULT_PATH;
	int backlog = DEFAULT_BACKLOG;
	struct sockaddr_un addr;
	int fd, ret;

	if (argc == 2)
		path = argv[1];
	if (argc == 3)
		backlog = atoi(argv[2]);

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1)
		err(EXIT_FAILURE, "cannot create a socket");

	/* cleanup the path first */
	ret = unlink(path);
	if (ret == -1 && errno != ENOENT)
		err(EXIT_FAILURE, "cannot unlink '%s'", path);

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
	ret = bind(fd, (struct sockaddr *)&addr, alen);
	if (ret == -1)
		err(EXIT_FAILURE, "cannot bind to '%s'", path);

	ret = listen(fd, backlog);
	if (ret == -1)
		err(EXIT_FAILURE, "listen error with backlog(%d)", backlog);

	for (;;) {
		char ibuf[BUFSIZ];
		ssize_t n, ret;
		int cfd;

		cfd = accept(fd, NULL, NULL);
		if (cfd == -1)
			err(EXIT_FAILURE, "accept error");

		/* dump the message to the stdout */
		while ((n = read(cfd, ibuf, BUFSIZ)) > 0)
			if ((ret = write(STDOUT_FILENO, ibuf, n)) != n)
				err(EXIT_FAILURE, "partial/failed write(%ld)", ret);

		if (n == -1)
			err(EXIT_FAILURE, "read error");
	}
	ret = close(fd);
	if (ret == -1)
		err(EXIT_FAILURE, "listen socket close error");

	ret = remove(path);
	if (ret == -1)
		err(EXIT_FAILURE, "cannot cleanup '%s'", path);

	exit(EXIT_SUCCESS);
}
