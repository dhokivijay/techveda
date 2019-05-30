
/* svshm_xfr_reader.c

   Read data from a System V shared memory using a binary semaphore lock-step
   protocol; see svshm_xfr_writer.c
*/
#include "svshm_xfr.h"

#define DEATH(mess) { perror(mess); exit(errno); }
int main()
{
    int semid, shmid, xfrs, bytes;
    struct shmseg *shmp;

    /* Get IDs for semaphore set and shared memory created by writer */

    semid = semget(SEM_KEY, 0, 0);
    if (semid == -1)
        DEATH ("semget");

    shmid  = shmget(SHM_KEY, 0, 0);
    if (shmid == -1)
        DEATH("shmget");

    /* Attach shared memory read-only, as we will only be reading */

    shmp = shmat(shmid, NULL, SHM_RDONLY);
    if (shmp == (void *) -1)
        DEATH ("shmat");

    /* Transfer blocks of data from shared memory to stdout */

    for (xfrs = 0, bytes = 0; ; xfrs++) {
        if (reserveSem(semid, READ_SEM) == -1)          /* Wait for our turn */
            DEATH ("reserveSem");

        if (shmp->cnt == 0)                     /* Writer encountered EOF */
            break;
        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
            perror("partial/failed write");

        if (releaseSem(semid, WRITE_SEM) == -1)         /* Give writer a turn */
            DEATH("releaseSem");
    }

    if (shmdt(shmp) == -1)
        perror("shmdt");

    /* Give writer one more turn, so it can clean up */

    if (releaseSem(semid, WRITE_SEM) == -1)
        perror("releaseSem");

    fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
