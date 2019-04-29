#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/timerfd.h>
#include <time.h>
#include <stdint.h>                     /* Definition of uint64_t */

int main()
{
    struct itimerspec ts;
    struct timespec start, now;
    int maxExp, fd, secs, nanosecs;
    uint64_t numExp, totalExp;
    ssize_t s;

    ts.it_interval.tv_sec = 2;
    ts.it_interval.tv_nsec = 10;
    ts.it_value.tv_sec = 1;
    ts.it_value.tv_nsec = 20;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        perror("timerfd_create");

    if (timerfd_settime(fd, 0, &ts, NULL) == -1)
        perror("timerfd_settime");

    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        perror("clock_gettime");

    for (totalExp = 0; totalExp < maxExp;) {

        /* Read expirations on the timer, and then display
           time elapsed since timer was started, followed by number
           of expirations read and total expirations so far. */

        s = read(fd, &numExp, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            perror("read");

        totalExp += numExp;

        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
            perror("clock_gettime");

        secs = now.tv_sec - start.tv_sec;
        nanosecs = now.tv_nsec - start.tv_nsec;
        if (nanosecs < 0) {
            secs--;
            nanosecs += 1000000000;
        }

        printf("%d.%03d: expirations read: %llu; total=%llu\n",
                secs, (nanosecs + 500000) / 1000000,
                (unsigned long long) numExp, (unsigned long long) totalExp);
    }
	
	return 0;

}
