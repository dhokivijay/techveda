#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define CHILD 0

int childstatus;

void sighand(int signum)
{
	printf(" I am in sig handler \n");
	wait(NULL);
}

int main()
{
	pid_t cpid;

	struct sigaction act;
	cpid = fork();
	if (cpid == CHILD) {
		printf(" I am in child task %d \n", getpid());
		sleep(2);
		//		while(1);
		exit(100);
	}

	else {

		act.sa_flags = SA_NOCLDWAIT;
		act.sa_handler = SIG_DFL; 

		/* Disable other state change notifications(SIGSTOP,SIGCONT) */
		//		 act.sa_flags = SA_NOCLDSTOP; 
		//                 act.sa_handler = sighand;

		if (sigaction(SIGCHLD, &act, NULL) == -1) 
			perror("sigaction: ");
		while (1) ;
	}
	return 0;
}

