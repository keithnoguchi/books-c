/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>

static char global[65536];
int primes[] = {2, 3, 5, 7};

static int square(int val)
{
	int result = val * val;
	printf("&val(stack):\t%p\n&result(stack):\t%p\n", &val, &result);
	return result;
}

static void do_calc(int val)
{
	int s = square(val);
	printf("&val(stack):\t%p\n&s(stack):\t%p\n", &val, &s);
	if (val < 10000) {
		int t = val * val * val;
		printf("t(stack):\t%p\n", &t);
	}
}

int main(void)
{
	static int key = 9973;
	static char buf[1024000000];
	char *p;

	p = malloc(1024); /* heap */
	printf("global(bss):\t%p\nprimes(data):\t%p\n&key(data):\t%p\n"
	       "buf(bss):\t%p\n&p(stack):\t%p\np(heap):\t%p\n",
	       global, primes, &key, buf, &p, p);
	do_calc(key);
	exit(EXIT_SUCCESS);
}
