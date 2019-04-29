/*

   Unlink a POSIX named semaphore.

*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#define DEATH(mess) { perror(mess); exit(errno); }
int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0){
        printf("%s sem-name\n", argv[0]);
	exit(1);
      }

    if (sem_unlink(argv[1]) == -1)
        DEATH ("sem_unlink");
    exit(EXIT_SUCCESS);
}
