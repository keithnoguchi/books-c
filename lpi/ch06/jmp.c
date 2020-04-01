/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

static jmp_buf env;

static void f2(void)
{
	longjmp(env, 2);
}

static void f1(int argc)
{
	if (argc == 1)
		longjmp(env, 1);
	f2();
}

static void usage(const char *const progname)
{
	printf("usage: %s [optional argument to call call f2()]\n", progname);
	exit(EXIT_SUCCESS);
}

int main(int argc, const char *const argv[])
{
	if (argc > 1 && !strcmp(argv[1], "--help"))
	    usage(argv[0]);
	switch (setjmp(env)) {
	case 0:
		printf("Calling f1() after initial setjmp()\n");
		f1(argc);
		break;
	case 1:
		printf("We jumped back from f1()\n");
		break;
	case 2:
		printf("We jumped back from f2()\n");
		break;
	}
	exit(EXIT_SUCCESS);
}
