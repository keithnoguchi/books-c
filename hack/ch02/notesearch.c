/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

/* progname name holder */
const char *progname;

#define FILENAME "/var/notes"

static int find_user_note(int fd, int uid)
{
	int note_uid = -1;
	int ret, n;
	char byte;

	/*
	 * find the note entry matches the uid and
	 * returns the length of the notes for that
	 * entry.
	 */
	while (note_uid != uid) {
		/* read uid header */
		ret = read(fd, &note_uid, 4);
		if (ret == -1)
			goto perr;
		if (ret == 0)
			break;
		if (ret != 4) {
			fprintf(stderr, "%s: wrong read length(%d)\n",
				progname, ret);
			goto err;
		}
		/* read newline separator. */
		ret = read(fd, &byte, 1);
		if (ret == -1)
			goto perr;
		if (ret == 0)
			break;
		if (ret != 1) {
			fprintf(stderr, "%s: wrong uid separator(%d)\n",
				progname, ret);
			goto err;
		}
		/* read one line. */
		byte = n = 0;
		while (byte != '\n') {
			ret = read(fd, &byte, 1);
			if (ret == -1)
				goto perr;
			else if (ret == 0)
				break;
			else if (ret != 1) {
				fprintf(stderr, "%s: wrong note read(%d)\n",
					progname, ret);
				goto err;
			}
			n++;
		}
		printf("note_uid=%d, uid=%d\n", note_uid, uid);
	}
	ret = lseek(fd, n * -1, SEEK_CUR);
	if (ret == -1)
		goto err;
	return n;
perr:
	fprintf(stderr, "%s:%s: %s\n", progname, __FUNCTION__, strerror(errno));
err:
	return -1;
}

static bool print_notes(int fd, int uid, const char *const needle)
{
	int nr;

	nr = find_user_note(fd, uid);
	if (nr == -1)
		return false;
	return true;
}

int main(int argc, const char *const argv[])
{
	int ret = EXIT_FAILURE;
	char needle[100];
	int fd, uid;

	progname = argv[0];
	if (argc < 2)
		needle[0] = '\0';
	else
		strncpy(needle, argv[1], sizeof(needle));

	uid = getuid();
	fd = open(FILENAME, O_RDONLY);
	if (fd == -1)
		goto err;

	/* keep searching a needle */
	while (print_notes(fd, uid, needle))
		;

	ret = EXIT_SUCCESS;
err:
	fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	if (fd != -1)
		close(fd);
	exit(ret);
}
