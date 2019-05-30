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
	struct itimerspec interval,user;
	int ret, fd, fd1;
	uint64_t exp, exp1;

	fd = timerfd_create(CLOCK_REALTIME, 0);
	fd1 = timerfd_create(CLOCK_REALTIME, 0);


	interval.it_value.tv_sec = 10;
	interval.it_value.tv_nsec = 0;
	interval.it_interval.tv_sec = 10;
	interval.it_interval.tv_nsec = 0;

	ret = timerfd_settime(fd, 0, &interval, NULL);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}


	user.it_value.tv_sec = 5;
	user.it_value.tv_nsec = 0;
	user.it_interval.tv_sec = 5;
	user.it_interval.tv_nsec = 0;

	ret = timerfd_settime(fd1, 0, &user, NULL);
	if (ret < 0) {
		perror("timer\n");
		exit(1);
	}

	while (1) {
		ret = read(fd, &exp, sizeof(uint64_t));
		printf("%s: timer counter %ld\n", __func__, exp);
		time(&curr);
		printf("%s: current time %s\n", __func__, ctime(&curr));

		ret = read(fd1, &exp1, sizeof(uint64_t));
		printf("%s: timer counter %ld\n", __func__, exp1);
		time(&curr);
		printf("%s: current user time %s\n", __func__, ctime(&curr));
	}
	return 0;		// should never reach here
}

/*
TODO:
1. modify above program code to register one more timer at 7 sec interval
 */

