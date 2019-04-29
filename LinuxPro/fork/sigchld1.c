#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define CHILD 0

int childstatus;
pid_t cpid, cpid1;

void sighand(int signum)
{
	printf("%s: destroying process %d\n", __func__, cpid);
	waitpid(cpid, NULL, 0);
}

int main()
{
	cpid = fork();
	if (cpid == CHILD) {
		printf(" I am in child 1 my pid is %d \n", getpid());
		sleep(10);
		exit(100);
	} else {
		
		signal(SIGCHLD, sighand);
		cpid1 = fork();
		if (cpid1 == CHILD) {
			printf("In child 2 my pid is %d\n", getpid());
			sleep(10);
			exit(0);
		} else {
			waitpid(cpid1, NULL, 0);
			while (1) {
				printf(" in loop \n");
				sleep(3);
			}
		}
	}
	return 0;
}
