/* 
sample application that registers 2 posix interval timers 
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

#define _POSIX_C_SOURCE 199309
#define BUF_SIZE 1000

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

char * currTime(const char *);

static void handler(int sig)
{
    printf("[%s] Got signal %d\n", currTime("%T"), sig);
}

int main()
{
    struct itimerspec ts;
    timer_t tid, tid2;
    int j;
    struct sigaction sa;

    sa.sa_handler = handler;  
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        perror("sigaction");

    if (timer_create(CLOCK_REALTIME, NULL, &tid) == -1)
        perror("timer_create");
    printf("timer ID = %ld\n", (long) tid);


    if (timer_create(CLOCK_REALTIME, NULL, &tid2) == -1)
        perror("timer_create");
    printf("timer ID = %ld\n", (long) tid2);

    ts.it_value.tv_sec = 5;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 5; // periodic interval ( 0 single shot)
    ts.it_interval.tv_nsec = 0;

    if (timer_settime(tid, 0, &ts, NULL) == -1)
        perror("timer_settime");

    ts.it_value.tv_sec = 12;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 12; // periodic interval ( 0 single shot)
    ts.it_interval.tv_nsec = 0;

    if (timer_settime(tid2, 0, &ts, NULL) == -1)
        perror("timer_settime");


    for (j = 0; ; j++)
    {
	
    	printf("[%s]", currTime("%T"));
        pause();
    }
}

/* Return a string containing the current time formatted according to
 * the specification in 'format' (see strftime(3) for specifiers).
 * If 'format' is NULL, we use "%c" as a specifier (which gives the'
 * date and time as for ctime(3), but without the trailing newline).
 * Returns NULL on error. */

char * currTime(const char *format)
{
    static char buf[BUF_SIZE];  
    time_t t;
    size_t s;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL)
        return NULL;

    s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

    return (s == 0) ? NULL : buf;
}

