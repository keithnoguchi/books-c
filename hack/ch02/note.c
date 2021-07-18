/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void usage(const char *const progname, const char *fname)
{
	printf("usage: %s <data to add to '%s'>\n", progname, fname);
	exit(EXIT_FAILURE);
}

static void fatal(const char *const progname)
{
	fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[])
{
	const char *const progname = argv[0];
	size_t flen = strlen("/tmp/note")+1;
	char *p, *buf, *fname;
	int ret = EXIT_FAILURE;
	int fd = -1;
	int i;

	buf = malloc(BUFSIZ);
	if (!buf)
		fatal(progname);
	fname = malloc(flen);
	if (!fname)
		fatal(progname);
	strncpy(fname, "/tmp/note", flen);
	if (argc < 2)
		usage(progname, fname);

	/* prepare the buffer with the note */
	p = buf;
	for (i = 1; i < argc; i++) {
		strncat(p, argv[i], BUFSIZ-(int)(p-buf));
		p += strlen(argv[i]);
		*(p++) = ' ';
	}
	strncat(--p, "\n", 2);

	/* open the temp file */
	fd = open(fname, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if (fd == -1)
		goto error;
	/* no retry */
	if (write(fd, buf, strlen(buf)) == -1)
		goto error;
	goto done;
error:
	fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	free(fname);
	free(buf);
done:
	if (fd != -1)
		close(fd);
	exit(ret);
}
