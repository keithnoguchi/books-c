/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>

int main(void)
{
	int a, b;
	float c, d;

	a = 13;
	b = 5;

	c = a / b;
	d = (float) a / (float) b;

	printf("[integers]\t a = %d\t b = %d\n", a, b);
	printf("[floats]\t c = %f\t d = %f\n", c, d);
}
