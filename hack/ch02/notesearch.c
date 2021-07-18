/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

/* progname name holder */
const char *progname;

#define FILENAME "/var/notes"

/* find the note and set the read pointer to the beginning of the note */
static int find_user_note(int fd, int uid)
{
	int note_uid = -1;
	int ret, n = 0;
	char byte;

	 /* find the note with the matched uid */
	while (note_uid != uid) {
		/* header */
		ret = read(fd, &note_uid, 4);
		if (ret == -1)
			goto perr;
		else if (ret == 0)
			break;
		else if (ret != 4) {
			fprintf(stderr, "%s: wrong read length(%d)\n",
				progname, ret);
			goto err;
		}
		/* header/body separator */
		ret = read(fd, &byte, 1);
		if (ret == -1)
			goto perr;
		else if (ret == 0)
			break;
		else if (ret != 1) {
			fprintf(stderr, "%s: wrong uid separator(%d)\n",
				progname, ret);
			goto err;
		}
		/* body */
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
	}
	if (n)
		if (lseek(fd, n * -1, SEEK_CUR) == -1)
			goto err;
	return n;
perr:
	fprintf(stderr, "%s:%s: %s\n", progname, __FUNCTION__, strerror(errno));
err:
	return -1;
}

/* search needle in the hay */
static bool search_note(const char *const hay, const char *const needle)
{
	int i, j;

	/* search needle in the note */
	for (i = 0; i < strlen(hay); i++) {
		if (hay[i] == needle[j])
			j++;
		else {
			if (hay[i] == needle[0])
				j = 1;
			else
				j = 0;
		}
		if (j == strlen(needle))
			return true;
	}
	return false;
}

static bool print_note(int fd, int uid, const char *const needle)
{
	char note[100];
	int nr, ret;

	nr = find_user_note(fd, uid);
	if (nr == -1)
		return false;
	else if (nr == 0)
		return false;
	ret = read(fd, note, nr);
	if (ret == -1)
		goto err;
	else if (ret == 0)
		return false;
	note[ret] = '\0';
	if (search_note(note, needle))
		printf(note);
	return true;
err:
	fprintf(stderr, "%s:%s: %s\n", progname, __FUNCTION__,
		strerror(errno));
	return false;
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

	/* keep printing the matched notes */
	while (print_note(fd, uid, needle))
		;

	ret = EXIT_SUCCESS;
err:
	if (ret == EXIT_FAILURE)
		fprintf(stderr, "%s:%s: %s\n", progname, __FUNCTION__, strerror(errno));
	if (fd != -1)
		close(fd);
	exit(ret);
}
