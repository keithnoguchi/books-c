/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <string.h>

int main(void)
{
	char message[14];
	int count, i;

	strcpy(message, "Hello, world!");

	printf("Repeat how many times? ");
	scanf("%d", &count);

	for (i = 0; i < count; i++)
		printf("%3d - %s\n", i, message);
}
