
/* psem_timedwait.c

   Decrease the value of a POSIX named semaphore using sem_timedwait().

   On Linux, named semaphores are supported with kernel 2.6 or later, and
   a glibc that provides the NPTL threading implementation.
*/
#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define DEATH(mess) { perror(mess); exit(errno); }

#define NAME "/mysem"
int main()
{
    sem_t *sem;
    struct timespec ts;


    sem = sem_open(NAME, 0);
    if (sem == SEM_FAILED)
        DEATH ("sem_open");

    /* sem_timedwait() expects an absolute time in its second argument.
       So we take the number of (relative) seconds specified on the
       command line, and add it to the current system time. */

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        DEATH ("clock_gettime-CLOCK_REALTIME");

    ts.tv_sec += 10;

    if (sem_timedwait(sem, &ts) == -1)
        DEATH ("sem_timedwait");

    printf("%ld sem_wait() succeeded\n", (long) getpid());
    exit(EXIT_SUCCESS);
}
