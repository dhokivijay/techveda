
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "rtc.h"

int main(void)
{

	int fd;
	struct rtc_time time;

	fd = open("/dev/myrtc", O_RDWR);
	if (fd < 0) {
		perror("open call");
		return -1;
	}

	read(fd, &time, sizeof(struct rtc_time));
	
	printf("sec : %x\n", time.sec);
	printf("min : %x\n", time.min);
	printf("hour: %x\n", time.hour);
	printf("day : %x\n", time.day);
	printf("mon : %x\n", time.mon);
	printf("year: %x\n", time.year);

	close(fd);
	return 0;
}


