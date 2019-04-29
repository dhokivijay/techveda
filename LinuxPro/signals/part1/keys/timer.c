#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

void alarm_hand(int signum)
{
	time_t curr;
	printf("%s: alarm reported\n", __func__);
	time(&curr);
	printf("%s: current time %s\n", __func__, ctime(&curr));
}

int main()
{
	struct itimerval itv;
	struct sigaction sa;
	sa.sa_handler = alarm_hand;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		perror("sigaction");

	itv.it_value.tv_sec = 5;	// initial event
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 10;	// periodic interval 
	itv.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
		perror("setitimer : ");
		exit(1);
	}
	while (1) {
		printf("%s:pointless message \n", __func__);
		sleep(1);
	}
	return 0;

}
