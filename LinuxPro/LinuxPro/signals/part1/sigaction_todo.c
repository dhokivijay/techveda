#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/* our signal handlers */

void handler(int what)
{
	printf("%s: Signal recieved %d\n",
	       __func__, what);
}

int main()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));

	/* Install signal handlers */
	act.sa_handler = handler;
	if (sigaction(SIGINT, &act, NULL) < 0)
	{
		perror("sigaction: \n");
		exit(1);
	}

	printf("%s: Successfully installed signal handler\n", __func__);

	/* Do something pointless, forever */
	for (;;) {
		printf("%s: This is a pointless message.\n", __func__);
		sleep(1);
	}

	return 0;
}
/* TODO:
   1. modify above code to restore default handler for SIGINT after 
      20 loop interations. (strictly using sigaction)
   2. set SIGINT to be ignored after 40 loop interations 
*/ 
