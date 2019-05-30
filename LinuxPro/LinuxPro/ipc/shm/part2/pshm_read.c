/* pshm_read.c
   
   Copy the contents of the POSIX shared memory object named in
   'name' to stdout.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define NAME "/myshm"

int main()
{
    int fd;
    char *addr;
    struct stat sb;


    fd = shm_open(NAME , O_RDONLY, 0);    /* Open existing object */
    if (fd == -1){
        perror("shm_open");
	exit(1);
	}

    /* Use shared memory object size as length argument for mmap()
       and as number of bytes to write() */

    if (fstat(fd, &sb) == -1){
        perror("fstat");
	exit(1);
	}

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
	exit(1);
	}

    if (close(fd) == -1)                    /* 'fd' is mo longer needed */
        perror("close");

    write(STDOUT_FILENO, addr, sb.st_size);
    printf("\n");
    exit(EXIT_SUCCESS);
}
