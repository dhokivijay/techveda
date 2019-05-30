#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/* our signal handlers */

void handler(int what)
{
	printf("%s: Signal recieved %d\n", __func__, what);
}

int main()
{
	struct sigaction act, defact;
	int it, ret;

	memset(&act, 0, sizeof(act));

	/* Install signal handlers */
	act.sa_handler = handler;
	if (sigaction(SIGINT, &act, &defact) < 0) {
		perror("sigaction: \n");
		exit(1);
	}

	printf("%s: Successfully installed signal handler\n", __func__);

	/* Do something pointless, forever */
	for (it = 0; it < 100; it++) {
		printf("%s: iteration count %d \n", __func__, it);
		sleep(1);
		if (it > 20 && it <= 40) {
			ret = sigaction(SIGINT, &defact, NULL);
			if (ret < 0)
				perror("SIGACTION\n");

			}
			if (it > 40) {
				act.sa_handler = SIG_IGN;
				ret = sigaction(SIGINT, &act, NULL);
				if (ret < 0)
					perror("sigaction\n");
			}
		}

	return 0;
}

/* TODO:
   1. modify above code to restore default handler for SIGINT after 
      20 loop interations. (strictly using sigaction)
   2. set SIGINT to be ignored after 40 loop interations 
*/
