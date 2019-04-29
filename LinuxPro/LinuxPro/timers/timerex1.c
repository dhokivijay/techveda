/* sample application that registers 2 posix interval timers 
pseudo code

step 1: create timer_t instances
step 2: Allocate timer id for the first timer
step 3: register handler for SIGALRM using sigaction
step 4: schedule first timer to elapse at 5sec periodic interval
step 5: Allocate timer id for second timer
step 6: Schedule second timer to elapse at 12 sec periodic interval
step 7: implement signal handler to print current clock time 

Note:  Handle both timer events using SIGALRM handler.
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

static void handler(int sig, siginfo_t *si, void *uc)
{
    printf("Got signal %d\n", sig);

}

static void rthandler(int sig, siginfo_t *si, void *uc)
{
    printf("Got signal %d\n", sig);
}

int main()
{
    struct itimerspec ts;
    struct sigevent ev;
    timer_t tid, tid2;
    int j;
    struct sigaction sa, sa1;
 
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;  
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        perror("sigaction");

    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = rthandler;  
    if (sigaction(SIGRTMIN, &sa1, NULL) == -1)
        perror("sigaction");

    ev.sigev_notify = SIGEV_SIGNAL;
    ev.sigev_signo = SIGALRM;
    ev.sigev_value.sival_ptr = &tid;

    if (timer_create(CLOCK_REALTIME, &ev, &tid) == -1)
        perror("timer_create");
    printf("timer ID = %ld\n", (long) tid);

    ev.sigev_notify = SIGEV_SIGNAL;
    ev.sigev_signo = SIGRTMIN;
    ev.sigev_value.sival_ptr = &tid2;

    if (timer_create(CLOCK_REALTIME, &ev, &tid2) == -1)
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


    for (j = 0; ; j++);
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

