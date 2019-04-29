#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define APIC_BASE 0xFEC00000


int main()
{
	int fd, ret;
	int *devmem = NULL;
	int *ioregsel, *iowin;
	int version;

	printf("pid %d\n", getpid());
	fd = open("/dev/mem", O_RDWR);

	devmem = (int *)mmap((void *)0, 4096, PROT_READ | PROT_WRITE,
			      MAP_PRIVATE, fd, APIC_BASE);
	if (devmem == NULL) {
		perror("Maping Failed");
		exit(1);
	}

	printf(" value in register %08x\n",(int) (*devmem)); 

	ioregsel = devmem;
	iowin = (int *)(devmem + 0x10);

	*ioregsel = 0x1;
	version = (int)(*iowin);
	printf("%d\n", version);

	ret = munmap(devmem, 4096);

	if (ret < 0)
		printf(" failed to unmap\n");
	getchar();

	return 0;
}
