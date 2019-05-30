#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main()
{

	int a[10]={1,2,3,4,5},temp,j;
	for(int i=0;i<5;i++)
	{
		scanf("%d",&a[i]);
	}
	for(int i=0,j=5;i<j;i++,j--)
	{
	temp=a[i];
	a[i]=a[j];
	a[j]=temp;
	}
	
	
	for(int i=1;i<=5;i++)
	{
	printf("%d",a[i]);
	}

}
