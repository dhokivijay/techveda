#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define CHILD 0

int childstatus;

/* signal handler to handle SIGCHLD event */
void sighand(int signum)
{
	printf(" I am in sig handler \n");
	wait(NULL);
}

int main()
{
	pid_t cpid;
	cpid = fork();
	if (cpid == CHILD) {
		printf(" I am in child task %d \n", getpid());
		sleep(10);
		exit(100);
	}
	else {
		/* register signal handler with process manager */
		signal(SIGCHLD, sighand);

		while (1) 
		{
			printf(" in loop \n");
			sleep(3);
		}
	}
	return 0;
}

/* 
   void handler(int signum)
   {
	wait(NULL);
   }

   int main()
  {
   	cPid = fork();
   	if(cPid  == 0)
        	code that runs in child
   	else
	{
        	signal(SIGCHLD, handler);
        	code that runs in parent
	}
  }
*/
