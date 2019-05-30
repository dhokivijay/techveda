#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/io.h>
#include "rtc.h"

struct rtc_time clk;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static unsigned char get_rtc(unsigned char addr)
{
	outb(addr, ADDRESS_REG);
	return inb(DATA_REG);
}

void threadFunc(union sigval sv)
{
	int res;

	printf("%s: Thread create with id : %u\n", __func__,
	       (int)pthread_self());
	res = ioperm(ADDRESS_REG, 0x04, 1);	/* we can access ports 70,71,72,73 */
	if (res < 0) {
		perror("ioperm \n");
		pthread_exit(NULL);
	}
	res = pthread_mutex_lock(&mtx);
	if( res < 0)
		perror("lock\n");
	
	clk.sec = get_rtc(SECOND);
	clk.min = get_rtc(MINUTE);
	clk.hour = get_rtc(HOUR);
	clk.day = get_rtc(DAY);
	clk.mon = get_rtc(MONTH);
	clk.year = get_rtc(YEAR);

	pthread_mutex_unlock(&mtx);
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
	sev.sigev_notify_attributes = NULL;	/* pthread_attr_t structure */

	/* Create and start timer */

	ts.it_interval.tv_sec = 30;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 0;

	if (timer_create(CLOCK_REALTIME, &sev, &tid) == -1) {
		perror("timer_create");
		exit(1);
	}

	if (timer_settime(tid, 0, &ts, NULL) == -1)
		perror("timer_settime");

	for (;;) {
		pthread_mutex_lock(&mtx);
		printf
		    ("%s:current clk:dd:mm:yy:hh:mi:ss %x:%x:%x:%x:%x:%x\n",
		     __func__, clk.day, clk.mon, clk.year, clk.hour, clk.min,
		     clk.sec);
		pthread_mutex_unlock(&mtx);
		sleep(30);
	}
}
