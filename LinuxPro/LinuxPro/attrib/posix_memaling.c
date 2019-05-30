#include<stdio.h>
#include<stdlib.h>

int main()
{
	int i;
	void *p;
//	i = posix_memalign (&p, 32, 160);
	p = malloc(160);
	printf("\n %u \n",(unsigned int )p%32);
}
	
	
	
