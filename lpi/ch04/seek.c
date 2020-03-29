/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

static void usage(const char *const progname)
{
	printf("usage: %s <filename> [option]\n", progname);
	exit(0);
}

int main(int argc, const char *const argv[])
{
	const char *const progname = argv[0];
	const char *file;
	int i, fd, ap;
	ssize_t nr;
	char *buf;
	long len;

	if (argc < 3 || strcmp(argv[1], "--help") == 0)
		usage(progname);

	file = argv[1];
	fd = open(file, O_RDWR);
	if (fd == -1)
		goto err;

	/* command handler */
	for (ap = 2; ap < argc; ap++) {
		switch (argv[ap][0]) {
		case 'r':
		case 'R':
			len = strtol(&argv[ap][1], NULL, 10);
			if (len == LONG_MIN || len == LONG_MAX)
				goto err;
			buf = malloc(len);
			if (!buf)
				goto err;
			nr = read(fd, buf, len);
			if (nr == -1)
				goto err;
			else if (nr == 0) {
				printf("%s: end-of-file\n", file);
				continue;
			}
			printf("%s: ", argv[ap]);
			for (i = 0; i < nr; i++) {
				if (argv[ap][0] == 'r')
					printf("%c",
					       isprint((unsigned char)buf[i]) ? buf[i] : '?');
				else
					printf("%02x ", (unsigned int)buf[i]);
			}
			printf("\n");
			free(buf);
			break;
		case'w':
			nr = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
			if (nr == -1)
				goto err;
			printf("%s: wrote %ld bytes\n", argv[ap], (long) nr);
			break;
		case 's':
			len = strtol(&argv[ap][1], NULL, 10);
			if (len == LONG_MIN || len == LONG_MAX)
				goto err;
			if (lseek(fd, len, SEEK_SET) == -1)
				goto err;
			printf("%s: seek succeeded\n", argv[ap]);
			break;
		default:
			usage(progname);
			break;
		}
	}
	if (close(fd))
		goto err;
	return 0;
err:
	perror(progname);
	return EXIT_FAILURE;
}
