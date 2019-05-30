#include <stdio.h>
#include <malloc.h>

main()
{
	void *p;

	p = malloc(16);
	malloc_stats();
	printf("\n allocated bytes : %d \n",malloc_usable_size(p));

}
