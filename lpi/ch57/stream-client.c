/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER "/tmp/uds-stream"
#define BUFLEN 10

int main(int argc, const char *const argv[])
{
	const char *const progname = argv[0];
	const char *path = SERVER;
	struct sockaddr_un addr;
	int fd, ret;

	if (argc > 1)
		path = argv[1];

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1)
		goto err;

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1)
		goto err;

	for ( ; ; ) {
		char *ptr, buf[BUFLEN];
		ssize_t ret, remain;

		ret = read(STDIN_FILENO, buf, sizeof(buf));
		if (ret == -1)
			goto err;
		else if (ret == 0)
			break;
		for (ptr = buf, remain = ret; remain > 0; ptr += ret, remain -= ret) {
			ret = write(fd, ptr, remain); 
			if (ret == -1)
				goto err;
		}
	}
	if (close(fd) == -1)
		goto err;
	exit(EXIT_SUCCESS);
err:
	perror(progname);
	exit(EXIT_FAILURE);
}
