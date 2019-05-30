#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main()
{
	int i =100;
	void *p, *p1;

	/* Get heap allocation stats before allocating any heap space */
	printf("\n Before Malloc Call \n");
	malloc_stats();
	printf("\n");
	getchar();

	/* Allocate heap block and observe allocation stats */
	printf("\n After Malloc Call \n");
	p = malloc(i);
	
	printf("\n %p \n", p);
	getchar();
	malloc_stats();

	/* free allocated chunk and observe allocation stats */
	free(p);
	getchar();
	malloc_stats();
}
