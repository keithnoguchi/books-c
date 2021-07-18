/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>

int global_var;
int global_initialized_var = 5;

void f(void) {
	int stack_var;

	printf("[in f] stack_var:\t\t\t %p\n", &stack_var);
}

int main(void) {
	int stack_var;
	static int static_initialized_var = 5;
	static int static_var;
	int *heap_var_ptr;

	heap_var_ptr = (int *)malloc(4);

	printf("[in main] global_initialized_var:\t %p\n", &global_initialized_var);
	printf("[in main] static_initialized_var:\t %p\n\n", &static_initialized_var);
	printf("[in main] global_var:\t\t\t %p\n", &global_var);
	printf("[in main] static_var:\t\t\t %p\n\n", &static_var);
	printf("[in main] heap_var:\t\t\t %p\n\n", heap_var_ptr);
	printf("[in main] stack_var:\t\t\t %p\n\n", &stack_var);
	f();

	free(heap_var_ptr);
}
