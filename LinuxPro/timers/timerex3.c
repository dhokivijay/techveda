/* sample application that registers 2 posix interval timers 
psudo code

step 1: create timer_t instances
step 2: Allocate timer id for the first timer
step 3: register handler for SIGALRM using sigaction
step 4: schedule timer to elapse at 5sec periodic interval
step 5: Allocate timer id for second timer
step 6: Schedule timer to elapse at 12 sec periodic interval
step 7: implement signal handler to print current clock time 

Hint : man pages : timer_create(2), timer_settime(2), time(2), localtime(2)
*/















#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

timestamp(char *str)
{
	time_t t;
	time(&t);
	printf("The time %s is %s\n", str, ctime(&t));
}

int main()
{

	sigset_t waitset;
	struct itimerspec ts;
	timer_t tid;
	siginfo_t info;
	int j, result;

	sigemptyset(&waitset);
	sigaddset(&waitset, SIGALRM);
	sigprocmask(SIG_BLOCK, &waitset, NULL);

	if (timer_create(CLOCK_REALTIME, NULL, &tid) == -1)
		perror("timer_create");
	printf("timer ID = %ld\n", (long)tid);

	ts.it_value.tv_sec = 2;
	ts.it_value.tv_nsec = 0;
	ts.it_interval.tv_sec = 2;	// periodic interval ( 0 single shot)
	ts.it_interval.tv_nsec = 0;

	if (timer_settime(tid, 0, &ts, NULL) == -1)
		perror("timer_settime");

	for (j = 0;; j++) {
		result = sigwaitinfo(&waitset, &info);
		if (result < 0)
			printf("sigwaitinfo failed : \n");

		/* got alarm  */
		printf("sigwaitinfo() returned for signal %d\n", info.si_signo);
		timestamp("after sigwaitinfo()");
	}
}
