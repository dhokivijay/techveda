#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *last_valid_address;
void main()
{
	intptr_t inc = 100;
	int ret;
	int *p;

	/* grab the last valid address from the OS */

	last_valid_address = sbrk(0);
	printf("\n %p \n", last_valid_address);
	getchar();

	/* change the location of the program break using sbrk */
	last_valid_address = sbrk(inc);

	last_valid_address = sbrk(0);
	printf("\n %p \n", last_valid_address);
	getchar();
	
}
