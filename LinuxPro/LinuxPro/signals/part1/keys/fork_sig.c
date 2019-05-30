/* 
   Test case to verify if signal handlers are shared by parent and
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

void child_handler(int signum)
{
	printf("%s: in handler for sig %d\n", __func__, signum);
}
int main()
{

	int ret;
	pid_t cpid;
	struct sigaction act;

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
		getchar();
		act.sa_handler = child_handler;
		ret = sigaction(SIGINT, &act, NULL);
		if (ret < 0) {
			perror("sigaction  \n");
			exit(1);
		}
		sleep(10);
		
	} else {
		wait(NULL);
		return 0;
	}

}
