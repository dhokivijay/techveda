#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#define CHILD 0

int global;

int main()
{
	pid_t pid;
	int childstatus;
	global = 1;
	pid = fork();
	if (pid == CHILD) {
		printf(" in child global %d\n", global);
		global = 10;//cow
		printf(" in child global %d\n", global);
		exit(1);
	}
	// parent
	else {
		wait(&childstatus);
		printf("in parent global %d\n", global);
	}
	return 0;
}
