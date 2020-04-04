/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER "/tmp/uds-stream-server"
#define BACKLOG 5
#define BUFLEN 10

int main(int argc, const char *const argv[])
{
	const char *const progname = argv[0];
	const char *path = SERVER;
	struct sockaddr_un addr;
	char buf[BUFLEN];
	int fd;

	if (argc == 2)
		path = argv[1];

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1)
		goto err;

	if (remove(path) == -1 && errno != ENOENT)
		goto err;

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		goto err;

	if (listen(fd, BACKLOG) == -1)
		goto err;

	/* sequential client handling */
	for ( ; ; ) {
		ssize_t ret;
		int cfd;

		/* we don't track the peer address */
		cfd = accept(fd, NULL, NULL);
		if (cfd == -1)
			goto err;

		/* print out to the stdout */
		if ((ret = read(cfd, buf, sizeof(buf))) > 0)
			if (write(STDOUT_FILENO, buf, ret) == -1)
				goto err;

		/* cleanup */
		if (ret == -1)
			goto err;
		if (close(cfd) == -1)
			goto err;
	}
	exit(EXIT_SUCCESS);
err:
	perror(progname);
	exit(EXIT_FAILURE);
}
