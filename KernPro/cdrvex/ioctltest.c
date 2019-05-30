
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

	ioctl(fd, SET_DAY, 7);

	close(fd);
	return 0;
}


