/* thread_incr_psem.c

   Use a POSIX unnamed semaphore to synchronize access by two threads to
   a global variable.

*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define DEATH(mess) { perror(mess); exit(errno); }

static int glob = 0;
static sem_t sem;

static void *threadFunc(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;

    for (j = 0; j < loops; j++) {
      if (sem_wait(&sem) == -1)
            DEATH ("sem_wait");

        loc = glob;
        loc++;
        glob = loc;

      if (sem_post(&sem) == -1)
            DEATH ("sem_post");
    }

    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int loops, s;

    loops = 10000000;

    /* Initialize a thread-shared mutex with the value 1 */

    if (sem_init(&sem, 0, 1) == -1)
        DEATH("sem_init");

    /* Create two threads that increment 'glob' */

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0)
        DEATH( "pthread_create");
    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0)
        DEATH ("pthread_create");

    /* Wait for threads to terminate */

    s = pthread_join(t1, NULL);
    if (s != 0)
        DEATH ("pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        DEATH( "pthread_join");

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
