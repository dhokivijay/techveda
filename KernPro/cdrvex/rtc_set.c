
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "./rtc.h"

int main(void)
{

	int fd;
	struct rtc_time time;

	fd = open("/dev/myrtc", O_RDWR);
	if (fd < 0) {
		perror("open call");
		return -1;
	}
	
	/* set new time stamp */
	time.sec = 10;
	time.min = 10;
	time.hour = 10;
	write(fd, &time, sizeof(time));

	close(fd);
	return 0;
}


