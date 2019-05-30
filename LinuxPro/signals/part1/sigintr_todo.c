#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sighand(int signum)
{
	printf("%s: signal rec'd : %d\n",__func__, signum);
}

int main()
{
	signal(SIGINT, sighand);
	signal(SIGQUIT, sighand);
	signal(SIGTERM, sighand);
	signal(SIGUSR1, sighand);
	while (1) {
		sleep(1);
		printf("%s:in loop\n", __func__);
	}
	return 0;
}
/* TODO

   1. setup sighand routine as handler for SIGINT,SIGQUIT,SIGTERM,SIGUSR1
   2. customize handler to print following msgs
		SIGINT: "hello sigint"
		SIGQUIT: "hello sigquit"
		SIGUSR1: "Hello sigusr1"
 
*/
