#include  <stdlib.h>
#include  <stdio.h>
#include  <unistd.h>
#include  <fcntl.h>
#include  <sys/io.h>
char kb_arr[]="  1234567890-=  qwertyuiop[]  asdfghjkl;'   zxcvbnm,./";

int main()
{
	int fd, ret;
	unsigned nint;
	unsigned int code;
	ret = ioperm(0x60, 0x04, 1);
	if ((fd = open("/dev/uio0", O_RDONLY)) < 0) {
		perror("Failed to open /dev/uio0\n");
		exit(EXIT_FAILURE);
	}
	while (1){
		read(fd, &nint, sizeof(nint));
		code=inb(0x60);
		printf("\n%s:Interrupts: %u, Scancode: %u, eq_char: %c\n",__func__, nint,code,kb_arr[code&0x7f]);
	}
	return 0;
}
