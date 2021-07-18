/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <string.h>

int main(void)
{
	char string[10];
	int A = -73;
	unsigned int B = 31337;

	strcpy(string, "sample");
	printf("[A] Dec: %d, Hex: %x, Unsigned: %u\n", A, A, A);
	printf("[B] Dec: %d, Hex: %x, Unsigned: %u\n", B, B, B);
	printf("[field width on B]: 3: '%3u', 10: '%10u', '%08u'\n",
	       B, B, B);
	printf("[string] '%s' Address %p\n", string, string);

	printf("variable A is at address: %p\n", &A);
}
