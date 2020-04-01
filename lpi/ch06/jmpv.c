/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

static void jumper(int i, int j, int k)
{
	printf("Before longjmp():\ti=%d, j=%d, k=%d\n", i, j, k);
	longjmp(env, 1);
}

int main(void)
{
	int i;
	register int j; /* allocated in register if possible */
	volatile int k; /* avoid optimization */

	i = 111;
	j = 222;
	k = 333;

	if (setjmp(env) == 0) {
		i = 777;
		j = 888;
		k = 999;
		jumper(i, j, k);
	} else
		printf("After longjmp():\ti=%d, j=%d, k=%d\n", i, j, k);
	exit(EXIT_SUCCESS);
}
