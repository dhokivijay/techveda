#include<stdio.h>
#include<sched.h>
#include<stdlib.h>
#include <unistd.h>

#define STACK_SIZE 16000

int dosomething(void *arg)
{
	printf("%s: running with pid %d\n",__func__, getpid());
	getchar();
	return 0;
}

int main()
{
	void *child_stack;
	/*get memory for child stack */
	child_stack = malloc(STACK_SIZE);
	if (child_stack == NULL) {
		perror("\nError creating \n");
		exit(1);
	}
	
	printf("%s: running with pid %d\n",__func__, getpid());
	/*create new child with clone */
	clone(dosomething, child_stack + STACK_SIZE, CLONE_VM | CLONE_FILES,
	      NULL);
	/*
	  arg 1: start routine of child
	  arg 2: stack for child
	  arg 3: list of resources to be shared with parent
	  arg 4: address of data blob to be passed as arg for start routine	
       */
	while(1);
	return 0; // should never reach here
}
