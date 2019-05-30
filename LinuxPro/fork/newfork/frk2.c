#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define CHILD 0

int main()
{
	pid_t childPid;
	childPid = fork();
	if (childPid == CHILD) {
		printf("%d: hello from child\n",getpid());
//		pause();
//		exit(0);
	}
//	else
		printf("%d: hello from parent\n",getpid());

	return 0;
}

/*     
	#define CHILD 0
 
	childpid = fork();
	if (childPid == CHILD) 
		work to be executed in child
	else
		work to be executed in parent
*/





















