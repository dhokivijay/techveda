#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEATH(mess) { perror(mess); exit(errno); }

/* our signal handlers */

void handler(int what)
{
	printf("%s: received SIGINT Executing work\n", __func__);
	sleep(5);
	printf("%s: exit from handler\n",__func__);
}

int main()
{
	struct sigaction act;
	sigset_t sigmask;

	memset(&act, 0, sizeof(act));

	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGQUIT);
	sigaddset(&sigmask, SIGTERM);

	/* Install signal handlers */
	act.sa_handler = handler;
	act.sa_mask = sigmask;	

	if (sigaction(SIGINT, &act, NULL) < 0)
	{
		perror("sigaction \n");
		exit(1);
	}
	/* Do something pointless, forever */
	for (;;) {
		printf("%s: This is a pointless message.\n",__func__);
		sleep(1);
	}

	return 0;
}
