/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static void usage(const char *const progname)
{
	printf("usage: %s <input> <output>\n", progname);
	exit(0);
}

int main(int argc, char *const argv[])
{
	const char *const progname = argv[0];
	int ifd, ofd, flags, mode;
	char buf[BUFSIZ];
	ssize_t nr;

	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usage(progname);

	/* open input file */
	ifd = open(argv[1], O_RDONLY);
	if (ifd == -1)
		goto err;

	/* open output file */
	flags = O_CREAT|O_WRONLY|O_TRUNC;
	mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
	ofd = open(argv[2], flags, mode);
	if (ofd == -1)
		goto err;

	/* copy contents */
	while ((nr = read(ifd, buf, BUFSIZ)) > 0)
		if (write(ofd, buf, nr) != nr) {
			fprintf(stderr, "unexpected write length: %ld", nr);
			goto err;
		}
	/* read error check */
	if (nr == -1)
		goto err;

	/* clean up */
	if (close(ofd))
		goto err;
	if (close(ifd))
		goto err;
	return EXIT_SUCCESS;
err:
	perror(progname);
	return EXIT_FAILURE;
}
