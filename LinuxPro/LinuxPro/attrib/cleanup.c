#include<stdio.h>
#include<malloc.h>


#define clean_memory __attribute__((cleanup(cleanUp)))

void cleanUp(int **p)
{
	printf("\n %p \n",*p);
	free(*p);
}
void test(int size)
{
	
        int *p clean_memory ;
	int i=0;
	p = malloc(size);

	printf("\n %p \n",p);
}


int main()
{
	test(100);
	return 0;	
}
