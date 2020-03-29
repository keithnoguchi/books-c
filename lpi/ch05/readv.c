/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

static void usage(const char *const progname)
{
	printf("usage: %s <filename>\n", progname);
	exit(EXIT_SUCCESS);
}

int main(int argc, const char *const argv[])
{
	const char *const progname = argv[0];
	const char *path;
#define STR_SIZE 100
	char str[STR_SIZE];
	struct iovec iov[3];
	struct stat stat;
	ssize_t len, ret;
	int x, fd;

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		usage(progname);

	path = argv[1];
	fd = open(path, O_RDONLY);
	if (fd == -1)
		goto perr;

	len = 0;
	iov[0].iov_base = &stat;
	iov[0].iov_len = sizeof(stat);
	len += iov[0].iov_len;

	iov[1].iov_base = &x;
	iov[1].iov_len = sizeof(x);
	len += iov[1].iov_len;

	iov[2].iov_base = str;
	iov[2].iov_len = sizeof(str);
	len += iov[2].iov_len;

	ret = readv(fd, iov, 3);
	if (ret == -1)
		goto perr;
	if (ret != len) {
		fprintf(stderr,
			"unexpected readv length:\n\t- want: %ld\n\t-  got: %ld\n",
			len, ret);
		goto err;
	}
	if (close(fd) == -1)
		goto perr;
	printf("total bytes requested: %ld; bytes read: %ld\n",
	       len, ret);
	return 0;
perr:
	perror(progname);
err:
	exit(EXIT_FAILURE);
}
