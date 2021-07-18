/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	int i;

	printf("RAND_MAX is %d\n", RAND_MAX);
	srand(time(NULL));

	printf("random values from 0 to RAND_MAX\n");
	for (i = 0; i < 8; i++)
		printf("%d ", rand());
	printf("\n");
	printf("random values from 0 to 20\n");
	for (i = 0; i < 8; i++)
		printf("%d ", rand()%20+1);
	printf("\n");
	exit(EXIT_SUCCESS);
}
