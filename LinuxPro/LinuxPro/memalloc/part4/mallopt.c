#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main()
{
	int i =100;
	void *p, *p1, *p2;


	mallopt(M_TOP_PAD, 0);
	mallopt(M_MMAP_THRESHOLD, 4096);		
	malloc_stats();
	getchar();

	p = (void *)malloc(4096 * 2);
	malloc_stats();
	getchar();

	free(p);
	malloc_stats();
	getchar();

	p1 = (void *)malloc(1024);
	malloc_stats();
	getchar();

	free(p1);
	malloc_stats();
	getchar();


	p2 = (void *)malloc(1024);
	malloc_stats();
	getchar();

	free(p2);
	malloc_stats();
	getchar();

	return 0;
}
