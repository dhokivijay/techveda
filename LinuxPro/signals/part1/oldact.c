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
	printf("We have received SIGALRM: Executing periodic work\n");
	alarm(5);
}

int main()
{
	struct sigaction newact, curract;

	memset(&newact, 0, sizeof(newact));

	/* Install signal handler */
	newact.sa_handler = handler;
	if (sigaction(SIGALRM, &newact, NULL) < 0)
		DEATH("sigaction");

	/* Examine current handler */
	if (sigaction(SIGALRM, NULL, &curract) < 0)
		DEATH("sigaction");

	if (curract.sa_handler == handler)
		printf("handler successfully installed \n");

	alarm(5);

	for (;;) {
		printf("This is a pointless message.\n");
		sleep(1);
	}

	return 0;
}
