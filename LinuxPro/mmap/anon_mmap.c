#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
	int *addr;		/* Pointer to shared memory region */
	pid_t cpid;

	/* Parent creates mapped region prior to calling fork() */


	addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED)
		perror("mmap");

	*addr = 1;		/* Initialize integer in mapped region */

	cpid = fork();
	if (cpid < 0)
		perror("fork");

	if (cpid == 0) {	
		/* Child: increment shared integer */
		printf("Shared Integer value in Child context = %d\n", *addr);
		(*addr)++;
		if (munmap(addr, sizeof(int)) == -1)
			perror("munmap");
	}

	else {			
		/* Parent: wait for child to terminate */
		if (wait(NULL) == -1)
			perror("wait");
		printf("In parent, value = %d\n", *addr);
		if (munmap(addr, sizeof(int)) == -1)
			perror("munmap");
	}
	return 0;
}
