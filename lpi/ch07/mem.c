/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *const progname)
{
	printf("usage: %s num-alloc block-size [step [min [max]]]\n", progname);
	exit(EXIT_SUCCESS);
}

int main(int argc, const char *const argv[])
{
	if (argc < 3 || !strcmp(argv[1], "--help"))
		usage(argv[0]);
	exit(EXIT_SUCCESS);
}
