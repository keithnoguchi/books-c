/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>

int f1(void)
{
	return 1;
}

int f2(void)
{
	return 2;
}

int main(void)
{
	int (*f)(void);
	int value;

	f = f1;
	printf("f is %p\n", f);
	value = f();
	printf("value returned is %d\n", value);
	f = f2;
	printf("f is %p\n", f);
	value = f();
	printf("value returned is %d\n", value);

	return 0;
}
