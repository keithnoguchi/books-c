/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>

void f(int a, int b, int c, int d)
{
	int flag;
	char buffer[10];

	flag = 31337;
	buffer[0] = 'A';

	printf("flag = 0x%08x\n", flag);
	printf("buffer[0] = %c\n", buffer[0]);
}

int main(void)
{
	f(1, 2, 3, 4);
}
