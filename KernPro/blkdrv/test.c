#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int main(){
	int fd,i;
	char buf[20]={};
	fd=open("/dev/sbd0", O_RDWR);
	if(fd<=0){
		perror("TEST_FD");
		exit(1);
	}
	ioctl(fd,BLKGETSIZE,&i);
	printf("Result of ioctl: %d\n",i);
	write(fd,"Hello",20);
	read(fd,buf,20);
	printf("DATA: %s",buf);

	return 0;

}
