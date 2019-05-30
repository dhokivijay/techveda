/* 
   Remove the POSIX shared memory object identified by 'name'
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NAME "/myshm"

int main()
{

    if (shm_unlink(NAME) == -1)
        perror("shm_unlink");
    exit(EXIT_SUCCESS);
}
