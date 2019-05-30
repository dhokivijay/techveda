#include <signal.h>

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void sighand(int a, siginfo_t *si, void * p)
{

	time_t curr;
/*	1. print signal reported. */
	printf("%s: signal reported %d for timer %d\n", __func__, a, si->si_timerid);
	if( a == SIGRTMIN)
	printf("%s: message received: %s\n", __func__, (char *)si->si_value.sival_ptr);
/*	2. print current time. */
	time(&curr);
	printf("%s: current time %s\n", __func__, ctime(&curr));
}

int main()
{
	struct sigaction act;
	timer_t tid, tid1;
	struct sigevent sev;
	struct itimerspec interval, interval1;
	int ret;

	/*1. Register signal handler for SIGALRM.
	   man 2 sigaction */

	act.sa_sigaction = sighand;
	act.sa_flags = SA_SIGINFO;
	ret = sigaction(SIGALRM, &act, NULL);
	if (ret < 0) {
		perror("sigactin \n");
		exit(1);
	}

	act.sa_sigaction = sighand;
	act.sa_flags = SA_SIGINFO;
	ret = sigaction(SIGRTMIN, &act, NULL);
	if (ret < 0) {
		perror("sigactin \n");
		exit(1);
	}

	/*2. create a new timer using timer_create call & choose real clock.
	   man 2 timer_create */

	sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = "hello world";
	ret = timer_create(CLOCK_REALTIME, NULL, &tid);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}

	ret = timer_create(CLOCK_REALTIME, &sev , &tid1);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}

	/*3. initialize timeout interval and arm timer.
	   man 2 timer_settime */

	interval.it_value.tv_sec = 7;
	interval.it_value.tv_nsec = 0;
	interval.it_interval.tv_sec = 7;
	interval.it_interval.tv_nsec = 0;

	ret = timer_settime(tid, 0, &interval, NULL);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}
	

	interval1.it_value.tv_sec = 10;
	interval1.it_value.tv_nsec = 0;
	interval1.it_interval.tv_sec = 10;
	interval1.it_interval.tv_nsec = 0;

	ret = timer_settime(tid1, 0, &interval1, NULL);
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

