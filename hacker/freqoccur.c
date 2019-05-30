#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
void main(){

	char *arr,count[10]={0};
	int n,cnt=0,index=0,j;
	scanf("%d",&n);
	arr=(char *)malloc(n*(sizeof(int)));
	scanf("%s",arr);
	int k=0,i=0;
	while(arr[i]!=32)
	{
		if(arr[i]>=48 && arr[i]<=57)
		{
			printf(" \n %d \n  ",arr[i]);
			k=arr[i]-48;
			count[k]++;
		}
		i++;
	}

	for(int i=0;i<10;i++)
	printf("%d",count[i]);
}
