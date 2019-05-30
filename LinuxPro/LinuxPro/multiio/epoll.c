#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define COUNT     10
#define MILLISECS -1
#define NOFDS      2
#define SIZE	  4096

int main()
{
	int fd1, fd2, epfd, ready;
	struct epoll_event ev;
	struct epoll_event evlist[COUNT];
	char buf[10];
	int n, i;
	
	fd1 = open("./pone", O_RDWR);
	fd2 = open("./ptwo", O_RDWR);

	epfd = epoll_create(SIZE);

	printf("fd1 is %d & fd2 is %d \n", fd1, fd2);

	ev.events = EPOLLIN | EPOLLET;	/* Only interested in input events */
	ev.data.fd = fd1;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev) == -1)
		perror("epoll_ctl");

	ev.events = EPOLLIN;	/* Only interested in input events */
	ev.data.fd = fd2;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev) == -1)
		perror("epoll_ctl");

	/* Fetch items from the ready list of the  epoll instance */
	printf("About to epoll_wait()\n");

	ready = epoll_wait(epfd, evlist, NOFDS, MILLISECS);
	if (ready < 0) {
		perror("epoll_wait");
		exit(1);
	}
	printf("Ready: %d\n", ready);

	for (i = 0; i < ready; i++) {

		if (evlist[i].events & EPOLLIN) {
			printf("file descriptor return by epoll %d \n",
			       evlist[i].data.fd);
			n = read(evlist[i].data.fd, buf, 10);
			printf(" read %d from fd%d \n", n, evlist[i].data.fd);
		}
	}
	return 0;
}
