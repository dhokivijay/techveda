#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <mydata.h>

int main()
{
	int fd;
	struct mydata *addr;
	sem_t *sem;
	fd = shm_open(DATA, O_RDWR, 0);
	if (fd == -1) {
		perror("shm_open");
		exit(1);
	}

	addr =
	    (struct mydata *)mmap(NULL, sizeof(struct mydata),
				  PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	sem = sem_open(SEM, 0);
	if (sem == SEM_FAILED) {
		perror("sem ");
		exit(1);
	}

	sem_wait(sem);
	addr->a = 10;
	sleep(10);
	addr->b = 30;
	sem_post(sem);

	munmap(addr, sizeof(struct mydata));

	return 0;
}
