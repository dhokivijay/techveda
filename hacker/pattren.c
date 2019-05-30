#include<stdio.h>
void main(){

int i,j,n,k,p;
printf("enter the no\n");
scanf("%d",&n);

printf("\n");
k=n*2-1;

for(i=k;i>0;i--,printf("\n"))
{
for(j=n;j>0;j--)
{
	for(p=n;p>0;p--)
	{
		if(p>j)
			printf(" ");
		else 
			printf("%d ",j);
	}
}
}



/*
for(i=n;i>0;i--,printf("\n"))
{
	for(j=n;j>0;j--)
	{
		if(j>i)
			printf(" ");
		else
			printf(" %d",i);
	//	if(j<i)
	//		printf(" G ");
		if(j>i)
			printf("%d ",j);
		else
			printf(" ");
		if(j<i)	
			printf(" %d ",i);
//	for(j=n;j>0;j--)
//		if(j>i) printf("  %d",j);

	
	}


}*/
printf("\n");
printf("\n");
}

