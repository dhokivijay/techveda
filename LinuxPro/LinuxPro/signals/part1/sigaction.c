#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void handler(int signum)
{

	printf("%s: handler invoked\n", __func__);

}

int main()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));

	/* Install signal handlers */
	act.sa_handler = handler;
	if (sigaction(SIGINT, &act, NULL) < 0) {
		perror("sigaction \n");
		exit(1);
	}

	/* Do something pointless, forever */
	while (1) {
		printf("%s:pointless message\n", __func__);
		sleep(1);
	}

	return 0;
}
