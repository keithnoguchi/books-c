/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>

static void usage(const char *progname)
{
	printf("usage: %s <message> <# of times to repeat>\n",
	       progname);
	exit(1);
}

int main(int argc, const char *const argv[])
{
	int i, count;

	if (argc < 3)
		usage(argv[0]);

	count = atoi(argv[2]);
	printf("Repeating %d times...\n", count);
	for (i = 0; i < count; i++)
		printf("%3d - %s\n", i, argv[1]);
}
