/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, const char *const argv[])
{
	char *char_ptr;
	int *int_ptr;
	int mem_size;

	if (argc < 2)
		mem_size = 50;
	else
		mem_size = atoi(argv[1]);

	printf("\t[+] allocationg %d bytes of memory on the heap for char_ptr\n",
	       mem_size);
	char_ptr = malloc(mem_size);
	if (!char_ptr) {
		fprintf(stderr, "Error: char_ptr %s\n", strerror(errno));
		exit(1);
	}
	strncpy(char_ptr, "This memory is located on the heap.", mem_size);
	printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

	printf("\t[+] allocating 12 bytes of memory on the heap for int_ptr\n");
	int_ptr = malloc(12);
	if (!int_ptr) {
		fprintf(stderr, "Error: int_ptr %s\n", strerror(errno));
		exit(1);
	}
	*int_ptr = 31337;
	printf("int_ptr (%p) --> '%d'\n", int_ptr, *int_ptr);

	printf("\t[-] freeing char_ptr's heap memory...\n");
	free(char_ptr);

	printf("\t[+] allocating another 15 bytes for char_ptr\n");
	char_ptr = malloc(15);

	if (!char_ptr) {
		fprintf(stderr, "Error: char_ptr %s\n", strerror(errno));
		exit(1);
	}

	strncpy(char_ptr, "new memory", 15);
	printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

	printf("\t[-] freeing int_ptr's heap memory...\n");
	free(int_ptr);
	printf("\t[-] freeing char_ptr's heap memory...\n");
	free(char_ptr);
}
