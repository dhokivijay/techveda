/* pshm_write.c
   Copy 'string' into the POSIX shared memory object
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>


#define NAME "/myshm"

int main(int argc, char *argv[])
{
    int fd;
    size_t len;                 /* Size of shared memory object */
    char *addr;

    fd = shm_open(NAME , O_RDWR, 0);      /* Open existing object */
    if (fd == -1){
        perror("shm_open");
	exit(1);
	}

    len = strlen(argv[1]);

    if (ftruncate(fd, len) == -1){        /* Resize object to hold string */
        perror("ftruncate");
	exit(1);
	}

    printf("Resized to %ld bytes\n", (long) len);

    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        perror("mmap");

   if (close(fd) == -1)                    /* 'fd' is mo longer needed */
        perror("close");

    printf("copying %ld bytes\n", (long) len);
    memcpy(addr, argv[1], len);             /* Copy string to shared memory */
    exit(EXIT_SUCCESS);
}
