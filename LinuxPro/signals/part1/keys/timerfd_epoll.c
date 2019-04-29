#include <sys/timerfd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/epoll.h>

#define SIZE 4096
#define COUNT 2

int main()
{
	time_t curr;
	struct itimerspec interval, interval1;
	int ret, fd, fd1, epfd, ready;
	int i;
	uint64_t exp, exp1;
	struct timeval tv;
	struct epoll_event ev;
	struct epoll_event evlist[COUNT];

	epfd = epoll_create(SIZE);

	fd = timerfd_create(CLOCK_REALTIME, 0);
	fd1 = timerfd_create(CLOCK_REALTIME, 0);

	ev.events = EPOLLIN;	/* Only interested in input events */
	ev.data.fd = fd;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
		perror("epoll_ctl");

	ev.events = EPOLLIN;	/* Only interested in input events */
	ev.data.fd = fd1;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev) == -1)
		perror("epoll_ctl");

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
		ready = epoll_wait(epfd, evlist, 2, -1);
		if (ready < 0) {
			perror("epoll_wait");
			exit(1);
		}

		printf("%s: FD count ready %d\n", __func__, ready);
		for (i = 0; i < ready; i++) {
			if (evlist[i].events & EPOLLIN) {
				ret = read(evlist[i].data.fd, &exp, sizeof(uint64_t));
				printf("%s: FD%d timer expired counter =  %ld\n", __func__, evlist[i].data.fd, exp);
				time(&curr);
				printf("%s: current time %s\n", __func__,
						ctime(&curr));
			}
		}
	}
	return 0;		// should never reach here
}

/*
TODO:
1. modify above program code to register one more timer at 7 sec interval
 */
