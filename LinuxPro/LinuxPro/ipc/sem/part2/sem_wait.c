#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

#define NAME "/mysem"

#define DEATH(mess) { perror(mess); exit(errno); }

int main()
{
    sem_t *sem;

    sem = sem_open(NAME , 0);
    if (sem == SEM_FAILED)
        DEATH ("sem_open");

    if (sem_wait(sem) == -1)
        DEATH ("sem_wait");
   
    if (sem_wait(sem) == -1)
        DEATH ("sem_wait");

    printf("%ld sem_wait() succeeded\n", (long) getpid());
    exit(EXIT_SUCCESS);
}
