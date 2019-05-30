/* pshm_create.c
   Create a POSIX shared memory object with specified size and permissions.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NAME "/myshm"

int main()
{
    int opt, fd;
    mode_t perms;
    size_t size;
    void *addr;

    size = getpagesize();
    /* Create shared memory object and set its size */

    fd = shm_open(NAME , O_CREAT | O_RDWR , 666);
    if (fd == -1){
        perror("shm_open");
	exit(1);
     }

    if (ftruncate(fd, size) == -1){
        perror("ftruncate");
        exit(1);
       }

    /* Map shared memory object */

    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED){
        perror ("mmap");
	exit(1);
       }
    exit(EXIT_SUCCESS);
}
