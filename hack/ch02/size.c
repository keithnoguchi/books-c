/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>

int main(void)
{
	printf("The 'int' data type is\t\t\t %ld bytes\n", sizeof(int));
	printf("The 'unsigned int' data type is\t\t %ld bytes\n",
	       sizeof(unsigned int));
	printf("The 'short int' data type is\t\t %ld bytes\n",
	       sizeof(short int));
	printf("The 'unsigned short int' data type is\t %ld bytes\n",
	       sizeof(unsigned short int));
	printf("The 'long int' data type is\t\t %ld bytes\n",
	       sizeof(long int));
	printf("The 'long long int' data type is\t %ld bytes\n",
	       sizeof(long long int));
	printf("The 'float' data type is\t\t %ld bytes\n", sizeof(float));
	printf("The 'double' data type is\t\t %ld bytes\n", sizeof(double));
	printf("The 'char' data type is\t\t\t %ld bytes\n", sizeof(char));
	printf("The 'unsigned char' data type is\t %ld bytes\n",
	       sizeof(unsigned char));
}
