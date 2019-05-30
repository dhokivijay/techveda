#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sighand(int signum)
{
	printf("%s:handler invoked by signal %d\n",__func__, signum);
}

int main()
{
	signal(SIGINT, sighand);
	while (1) {
		sleep(2);
		printf("%s:in loop\n",__func__);
	}
	return 0;
}
/* TODO

   1. setup sighand routine as handler for SIGINT,SIGQUIT,SIGTERM,SIGUSR1
 
*/
