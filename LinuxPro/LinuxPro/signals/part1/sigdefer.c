#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void handler(int signum)
{
	printf("%s: We have received signal :  %d \n", __func__, signum);
	sleep(10);
	printf(" %s: exit from sighandler \n",__func__);
}

int main()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));

	act.sa_flags = SA_RESETHAND;
//	act.sa_flags = SA_NODEFER;
	act.sa_handler = handler;
	if (sigaction(SIGINT, &act, NULL) < 0)	
	{
		perror("sigaction \n");
		exit(errno);
	}

	/* Do something pointless, forever */
	while(1) {
		printf("%s: pointless message.\n",__func__);
		sleep(1);
	}
	return 0;
}
