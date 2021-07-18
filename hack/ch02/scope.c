/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>

int j = 42;

void f3(void)
{
	int i = 11, j = 999;
	printf("\t\t\t[in f3] i @ %p = %3d\n", &i, i);
	printf("\t\t\t[in f3] j @ %p = %3d\n", &j, j);
}

void f2(void)
{
	int i = 7;
	printf("\t\t[in f2] i @ %p = %3d\n", &i, i);
	printf("\t\t[in f2] j @ %p = %3d\n", &j, j);
	printf("\t\t[in f2] setting j = 1337\n");
	j = 1337;
	f3();
	printf("\t\t[back in f2] i @ %p = %3d\n", &i, i);
	printf("\t\t[back in f2] j @ %p = %3d\n", &j, j);
}

void f1(void)
{
	int i = 5;
	printf("\t[in f1] i @ %p = %3d\n", &i, i);
	printf("\t[in f1] j @ %p = %3d\n", &j, j);
	f2();
	printf("\t[back in f1] i @ %p = %3d\n", &i, i);
	printf("\t[back in f1] j @ %p = %3d\n", &j, j);
}

int main(void)
{
	int i = 3;
	printf("[in main] i @ %p = %3d\n", &i, i);
	printf("[in main] j @ %p = %3d\n", &j, j);
	f1();
	printf("[back in main] i @ %p = %3d\n", &i, i);
	printf("[back in main] j @ %p = %3d\n", &j, j);
}
