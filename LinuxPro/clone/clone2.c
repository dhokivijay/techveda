/* This will show you how to create a new process with clone and share data between processes*/

#include<stdio.h>
#include<sys/mman.h>
#include<sched.h>
#include<stdlib.h>

#define STACK_SIZE  16000

int variable;
int dosomething(void *arg)
{
	printf("%s: In child process \n",__func__);
	variable = 40;
	return 0;
}

int main()
{
	void *child_stack;
	variable = 10;
	/*get memory for child stack */
	printf("%s: Value of variable now is %d\n",__func__, variable);
	
	child_stack = malloc(STACK_SIZE);
	if (child_stack == NULL) {
		perror("\nError creating \n");
		exit(1);
	}
	/*create new child with clone */
	clone(dosomething, child_stack + STACK_SIZE, CLONE_VM | CLONE_FILES              ,NULL);
	/*sleep some time so that child can change 'variable' */
	sleep(2);
	printf("%s :Value of variable now is %d\n",__func__, variable);
}
