#include <sys/timerfd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>

int main()
{
	time_t curr;
	struct itimerspec interval, interval1;
	int ret, fd, fd1;
	uint64_t exp, exp1;
	fd_set rfds;
	struct timeval tv;
	int retval;

	fd = timerfd_create(CLOCK_REALTIME, 0);
	fd1 = timerfd_create(CLOCK_REALTIME, 0);

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	FD_SET(fd1,&rfds);

	interval.it_value.tv_sec = 5;
	interval.it_value.tv_nsec = 0;
	interval.it_interval.tv_sec = 10;
	interval.it_interval.tv_nsec = 0;

	ret = timerfd_settime(fd, 0, &interval, NULL);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}

	interval1.it_value.tv_sec = 7;
	interval1.it_value.tv_nsec = 0;
	interval1.it_interval.tv_sec = 7;
	interval1.it_interval.tv_nsec = 0;

	ret = timerfd_settime(fd1, 0, &interval1, NULL);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}

	while (1) {
		tv.tv_sec = 30;
		tv.tv_usec = 0;
		retval = select(4, &rfds, NULL, NULL, &tv);
		if (retval < 0) {
			perror("select \n");
			exit(1);
		}
		printf("%s: FD count ready %d\n", __func__, retval);

		if (FD_ISSET(fd, &rfds)) {
			ret = read(fd, &exp, sizeof(uint64_t));
			printf("%s: %d timer expired counter =  %ld\n", __func__, fd, exp);
			time(&curr);
			printf("%s: current time %s\n", __func__, ctime(&curr));
		}
		if (FD_ISSET(fd1, &rfds)) {
			ret = read(fd1, &exp1, sizeof(uint64_t));
			printf("%s: %d timer expired counter =  %ld\n", __func__, fd1, exp1);
			time(&curr);
			printf("%s: current time %s\n", __func__, ctime(&curr));
		}
	}
	return 0;		// should never reach here
}

/*
   TODO:
   1. modify above program code to register one more timer at 7 sec interval
*/
