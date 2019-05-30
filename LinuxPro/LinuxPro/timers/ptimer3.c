#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>

static void threadFunc(union sigval sv)
{
   printf("Thread create with id : %ld\n", pthread_self());
   pthread_exit(NULL);
}

int main()

{
	struct sigevent sev;
	timer_t tid;
	struct itimerspec ts;
	int s, j;

	sev.sigev_notify = SIGEV_THREAD;	/* Notify via thread */
	sev.sigev_notify_function = threadFunc;	/* Thread start function */
	sev.sigev_notify_attributes = NULL;	/* Could be pointer to pthread_attr_t structure */

	/* Create and start timer */

	ts.it_interval.tv_sec = 2;
	ts.it_interval.tv_nsec = 10;
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 20;

	if (timer_create(CLOCK_REALTIME, &sev, &tid) == -1) {
		perror("timer_create");
		exit(1);
	}

	if (timer_settime(tid, 0, &ts, NULL) == -1)
		perror("timer_settime");


	for (;;) {
	sleep(1);
	printf(" This is a pointless message\n");
	}
}
