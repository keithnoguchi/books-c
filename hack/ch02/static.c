/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>

void f(void)
{
	int var = 5;
	static int static_var = 5;

	printf("\t[in f] var @ %p = %d\n", &var, var);
	printf("\t[in f] static_var @ %p = %d\n", &static_var, static_var);
	var++;
	static_var++;
}

int main(void)
{
	int i;
	static int static_var = 1337;

	for (i = 0; i < 5; i++) {
		printf("[in main] static_var @ %p = %d\n",
		       &static_var, static_var);
		f();
	}
}
