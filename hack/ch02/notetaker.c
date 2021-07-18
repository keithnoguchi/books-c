/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static void usage(const char *const progname, const char *const fname)
{
	printf("usage: %s <note to append to %s>\n",
	       progname, fname);
	exit(EXIT_FAILURE);
}

int main(int argc, const char *const argv[])
{
	const char *const progname = argv[0];
	const char *const fname = "/var/notes";
	char *p, buf[BUFSIZ];
	int ret, uid, fd, i;

	if (argc < 2)
		usage(progname, fname);

	/* prepare the note */
	p = buf;
	for (i = 1; i < argc; i++) {
		strncat(p, argv[i], BUFSIZ-(p-buf));
		p += strlen(argv[i]);
		*(p++) = ' ';
	}
	*(p++) = '\n';

	fd = open(fname, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if (fd == -1)
		goto err;

	/* uid marker */
	uid = getuid();
	ret = write(fd, &uid, sizeof(uid));
	if (ret == -1)
		goto err;
	ret = write(fd, "\n", 1);
	if (ret == -1)
		goto err;
	ret = write(fd, buf, p-buf);
	if (ret == -1)
		goto err;
	ret = EXIT_SUCCESS;
	goto done;
err:
	ret = EXIT_FAILURE;
	fprintf(stderr, "%s: %s\n", progname, strerror(errno));
done:
	if (fd != -1)
		close(fd);
	exit(ret);
}
