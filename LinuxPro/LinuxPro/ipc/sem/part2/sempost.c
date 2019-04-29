/* 
   Increase the value of a POSIX named semaphore.

*/
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define NAME "/mysem"

int main()
{
    sem_t *sem;

    sem = sem_open(NAME, 0);
    if (sem == SEM_FAILED)
        perror("sem_open");

    if (sem_post(sem) == -1)
        perror("sem_post");
    exit(EXIT_SUCCESS);
}
