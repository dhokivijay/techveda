#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

void alarm_hand(int signum)
{
	time_t curr;
	printf("%s: alarm reported by %d \n", __func__, getpid());
	time(&curr);
	printf("%s: current time %s\n", __func__, ctime(&curr));
}

int main()
{
	struct itimerval itv;
	struct sigaction sa;
	pid_t cpid;

	sa.sa_handler = alarm_hand;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		perror("sigaction");

	itv.it_value.tv_sec = 10;	// initial event
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 10;	// periodic interval 
	itv.it_interval.tv_usec = 0;

	if(setitimer(ITIMER_REAL, &itv, NULL) == -1) 
	
		perror("setitimer : ");
		exit(1);
	}

	cpid = fork();
	if (cpid < 0) {
		perror("fork \n");
		exit(1);
	}
	if (cpid == 0) 
	
		while (1) {
			printf("%s:pointless message from %d \n", __func__,
			       getpid());
			sleep(1);
		}

	} else {
		while (1) {
			printf("%s:pointless message from %d \n", __func__,
			       getpid());
			sleep(1);
		}
	}
	return 0;

}
