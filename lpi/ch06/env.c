/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *const progname)
{
	printf("usage: %s [NAME=VALUE]...\n", progname);
	exit(EXIT_SUCCESS);
}

int main(int argc, const char *const argv[])
{
	extern const char *const *environ;
	const char *progname = argv[0];
	const char *const *p;

	if (argc > 1 && !strcmp(argv[1], "--help"))
		usage(progname);

	clearenv();

	for (p = argv; *p; p++)
		if (putenv((char *)*p))
			goto err;

	if (setenv("GREET", "Hello, world", 0))
		goto err;
	if (unsetenv("BYE"))
		goto err;

	for (p = environ; *p; p++)
		puts(*p);
	exit(EXIT_SUCCESS);
err:
	perror(progname);
	exit(EXIT_FAILURE);
}
