#include<stdio.h>
int *a;

void cons(void) __attribute__((constructor));
void dest(void) __attribute__((destructor));
void cons(void)
{
	int i = 0;
	printf("\n In constructor\n");
	a=(int *)malloc(20);
	for(;i<5;i++)
		a[i]=i+1;
	
}

void dest(void)
{
	printf("\n In destructor\n");
	free(a);
}

int main()
{
	int i;
	for(i=0;i<5;i++)
	printf("\n a = %d \n",a[i]);
}

