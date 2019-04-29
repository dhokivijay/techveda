#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <mtxmydata.h>

int main()
{
	int fd;
	unsigned int val;
	struct mydata *addr;
	pthread_mutexattr_t pshared;

	pthread_mutexattr_init(&pshared);
	pthread_mutexattr_setpshared(&pshared, PTHREAD_PROCESS_SHARED);
	pthread_mutexattr_getpshared(&pshared , &val);
	printf(" %d\n", val);

	/* set up shared memory node */
	fd = shm_open(DATA, O_CREAT | O_RDWR, 666);
	if (fd == -1) {
		perror("shm_open");
		exit(1);
	}

	/* assign buffer to node */
	if (ftruncate(fd, getpagesize()) == -1) {
		perror("ftruncate");
		exit(1);
	}

	/* map buffer to pspace */
	addr =
	    (struct mydata *)mmap(NULL, sizeof(struct mydata),
				  PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	pthread_mutex_init(&addr->mtxlock, &pshared);
	addr->a = 0;
	addr->b = 0;
	munmap(addr, sizeof(struct mydata));

	return 0;
}
