#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
void main()
{
	int *ptr=NULL;
	ptr=malloc(1);
	*ptr=1;

	int pid;

	if(fork()==0)
	{
		printf("in child process %d\n ",*ptr);
		(*ptr)++;
		//free(ptr);
	}
	else
	{
		if(wait(NULL)==-1) perror("wait");
		printf("in parent %d\n",*ptr);
	//	free(ptr);


	}
}
