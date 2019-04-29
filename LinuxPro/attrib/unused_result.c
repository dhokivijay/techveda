#include<stdio.h>
#include<malloc.h>
int myalloc(int,int ** ) __attribute__((warn_unused_result)) ;
int myalloc(int size,int **p)
{
	*p =  malloc(size);
	if(*p == NULL)
		return 0;
	else
		return 1;
}
int main()
{
	int *p;
	myalloc(100,&p)
	/* write data */
	return 0;	
}
