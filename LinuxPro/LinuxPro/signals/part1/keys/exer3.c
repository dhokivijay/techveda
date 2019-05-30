#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

void alarm_hand(int signum)
{
	printf(" %s: I am in sig handler %d \n", __func__, signum);
//      sleep(1);
}

int main()
{
	struct itimerval itv;
	struct sigaction act;

	act.sa_handler = alarm_hand;
	if (sigaction(SIGALRM, &act, NULL) == -1)
		perror("sigaction");

	itv.it_value.tv_sec = 10;
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 10;
	itv.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	while (1) {
		printf("%s: pointless message \n", __func__);
		sleep(1);
	}
	return 0;
}
