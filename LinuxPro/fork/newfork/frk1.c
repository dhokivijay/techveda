#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define CHILD 0

int main()
{
	pid_t childPid;
	childPid = fork();
	printf("%s:hello fork %d\n",__func__, getpid());
	return 0;
}
