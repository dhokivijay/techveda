#include<stdio.h>
#include<malloc.h>


void test(int size)
{

	int *p;
	int i = 0;
	printf("pid %d\n",getpid());
	getchar();
	p = malloc(size);
	printf("\n %p \n", p);
	getchar();
	free(p);
}

int main()
{
	test(128*4096);
	return 0;
}
