/* This will show you how to create a new process with clone and share open files  between processes*/
#include<stdio.h>
#include<sys/mman.h>
#include<sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#define STACK_SIZE 16000

int fd;
int dosomething(void *arg)
{
//	sleep(2);
	printf("\nIn child process\n");
	/*close the file */
	close(fd);
}

int main()
{
	int ret;
	void *child_stack;
	char buf[100]={};


	fd=open("./test",O_RDWR);
	if(fd==-1){
		perror("");
		exit(1);
	}

	/*get memory for child stack*/
	child_stack=malloc(STACK_SIZE);
	if(child_stack==NULL){
		perror("\nError creating \n");
		exit(1);
	}
	/*create new child with clone*/
	clone(dosomething,child_stack+STACK_SIZE,CLONE_VM|CLONE_FILES,NULL);
	
	sleep(2);
	
	ret=read(fd,buf,100);
	printf("%s: ret val%d\n",__func__,ret);
	if(ret<=0){
		perror("Read error");
		exit(1);
	}
	printf("%s: Data : %s\n",__func__, buf);
	wait(NULL);
}
