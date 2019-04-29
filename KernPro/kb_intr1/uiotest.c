#include  <stdlib.h>
#include  <stdio.h>
#include  <unistd.h>
#include  <fcntl.h>
#include  <sys/io.h>

int main()
{
	int fd, ret;
	unsigned nint;
	unsigned code;
	ret = ioperm(0x60, 0x04, 1);
	if ((fd = open("/dev/uio0", O_RDONLY)) < 0) {
		perror("Failed to open /dev/uio0\n");
		exit(EXIT_FAILURE);
	}
	while (1){
		read(fd, &nint, sizeof(nint));
		code=inb(0x60);
		printf("%s:Interrupts: %u, Scancode: %u\n",__func__, nint,code);
	}
	return 0;
}
