#include<stdio.h>
#include<malloc.h>

/* Compile with -Wnonnull */

void test(int *) __attribute__((nonnull));

void test(int *p)
{
	*p=100;	
}


int main()
{
	int a=10;
	test(NULL);
	return 0;	
}
