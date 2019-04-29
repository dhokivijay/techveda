/* 
   Try to decrease the value of a POSIX named semaphore using the
   nonblocking sem_trywait() function.

*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#define NAME "/mysem"
#define DEATH(mess) { perror(mess); exit(errno); }

int main(int argc, char *argv[])
{
    sem_t *sem;


    sem = sem_open(NAME, 0);
    if (sem == SEM_FAILED)
        DEATH ("sem_open");

    if (sem_trywait(sem) == -1)
        DEATH ("sem_trywait");

    exit(EXIT_SUCCESS);
}
