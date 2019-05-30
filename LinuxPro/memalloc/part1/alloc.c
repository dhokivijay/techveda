#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>

void dup(int *p,int size)
{
	int *temp,i;

	temp = alloca(size);
	getchar();
	memcpy(temp,p,size);
	
	for(i=0;i<5;i++)
		temp[i]=i+10;
	
}


int main()
{
	int a[5] = {1,2,3,4,5},i;

	dup(a,sizeof(a));
	for(i=0;i<5;i++)
		printf(" %d",a[i]);
	printf("\n");
	exit(EXIT_SUCCESS);
	
	
}
