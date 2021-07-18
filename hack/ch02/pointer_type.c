/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>

int main(void)
{
	char char_array[] = {'a', 'b', 'c', 'd', 'e'};
	int int_array[] = {1, 2, 3, 4, 5};
	void *ptr;
	int i;

	ptr = char_array;
	for (i = 0; i < sizeof(char_array)/sizeof(char); i++) {
		printf("[integer pointer] points to %p, which contains the char '%c'\n",
		       ptr, *(char *)ptr);
		ptr = (char *)ptr + 1;
	}

	ptr = int_array;
	for (i = 0; i < sizeof(int_array)/sizeof(int); i++) {
		printf("[char pointer] points to %p, which contains the int '%d'\n",
		       ptr, *(int *)ptr);
		ptr = (int *)ptr + 1;
	}
}
