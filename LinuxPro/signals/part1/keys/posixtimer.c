#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void sighand(int a)
{

	time_t curr;
/*	1. print signal reported. */
	printf("%s: signal reported %d\n", __func__, a);
/*	2. print current time. */
	time(&curr);
	printf("%s: current time %s\n", __func__, ctime(&curr));
}

int main()
{
	struct sigaction act;
	timer_t tid;
	struct itimerspec interval;
	int ret;

	/*1. Register signal handler for SIGALRM.
	   man 2 sigaction */

	act.sa_handler = sighand;
	ret = sigaction(SIGALRM, &act, NULL);
	if (ret < 0) {
		perror("sigactin \n");
		exit(1);
	}

	/*2. create a new timer using timer_create call & choose real clock.
	   man 2 timer_create */

	ret = timer_create(CLOCK_REALTIME, NULL, &tid);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}

	/*3. initialize timeout interval and arm timer.
	   man 2 timer_settime */

	interval.it_value.tv_sec = 10;
	interval.it_value.tv_nsec = 0;
	interval.it_interval.tv_sec = 10;
	interval.it_interval.tv_nsec = 0;

	ret = timer_settime(tid, 0, &interval, NULL);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}
	/* 4. push main thread into infinite loop (for testing timer) */
	while (1) {
		printf("%s in loop\n", __func__);
		sleep(1);
	}
	return 0;		// should never reach here
}

/*
   TODO:
   1. modify above program code to register one more timer at 7 sec interval
   2. Signal handler should print id of the timer event reported.

*/

