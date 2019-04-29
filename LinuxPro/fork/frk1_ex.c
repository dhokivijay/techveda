#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define CHILD 0

int main()
{
	int i;
	pid_t childPid, pid2;
	childPid = fork();

	if (childPid == CHILD) {
		printf("child: My pid = %d and ppid = %d\n", getpid(),
		       getppid());
		sleep(10);
	} else {
		pid2 = fork();
		if (pid2 == CHILD) {
			printf("child: My pid = %d and ppid = %d\n", getpid(),
			       getppid());
			sleep(10);
		} else {
			printf("PARENT :pid = %d\n", getpid());
			while (1) ;
		}
	}
	return 0;
}

/*      
	childpid = fork();
	if (childPid == CHILD) 
		work to be executed in child
	else
		work to be executed in parent
*/
