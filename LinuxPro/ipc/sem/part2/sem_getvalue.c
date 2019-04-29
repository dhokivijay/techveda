#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

#define NAME "/mysem"
#define DEATH(mess) { perror(mess); exit(errno); }

int main()
{
    int value;
    sem_t *sem;

    sem = sem_open(NAME, 0);
    if (sem == SEM_FAILED)
        DEATH ("sem_open");

    if (sem_getvalue(sem, &value) == -1)
        DEATH ("sem_getvalue");

    printf("%d\n", value);
    exit(EXIT_SUCCESS);
}
