/* 
   Test case to verify if signal mask is shared by parent and
   child processes created using fork 
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
void handler(int signum)
{
	printf("%s: in handler for sig %d\n", __func__, signum);
}

int main()
{
	int ret;
	pid_t cpid;
	sigset_t mask;
	struct sigaction act;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGUSR1);
	sigaddset(&mask, SIGTERM);

	ret = sigprocmask(SIG_BLOCK, &mask, NULL);
	if (ret < 0) {
		perror("sigprocmask  \n");
		exit(1);
	}

	act.sa_handler = handler;
	ret = sigaction(SIGINT, &act, NULL);
	if (ret < 0) {
		perror("sigaction  \n");
		exit(1);
	}

	cpid = fork();
	if (cpid < 0) {
		perror("fork \n");
		exit(1);
	}
	if (cpid == 0) {
		printf("%s:child code with pid %d\n", __func__, getpid());
		while (1) {
			printf("%s:pointless message\n", __func__);
			sleep(1);
		}
	} else {
		wait(NULL);
		return 0;
	}

}
