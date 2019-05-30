#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	int fd1, fd2;
	fd_set read_set, write_set, except_set;
	char buf[10];
	struct timespec timeout;

	sigset_t mask;
	int n;

	timeout.tv_sec = 10;
	timeout.tv_nsec = 0;

	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);

	fd1 = open("./pone", O_RDWR);
	fd2 = open("./ptwo", O_RDWR);

	while (1) {
		FD_ZERO(&read_set);
		FD_SET(fd1, &read_set);
		FD_SET(fd2, &read_set);
		n = pselect(FD_SETSIZE, &read_set, NULL, NULL, &timeout, &mask);

//		n = select(FD_SETSIZE, &read_set, NULL, NULL, &timeout);
		if (n < 0) {
			perror("select ");
			exit(1);
		}
		// test whether fd1 is ready or not
		if (FD_ISSET(fd1, &read_set)) {
			printf(" reading from fd1 (pone)\n");
			n = read(fd1, buf, 10);
			printf(" read %d from pone \n", n);
		}
		// test whether fd2 is ready or not
		if (FD_ISSET(fd2, &read_set)) {
			printf(" reading from fd2 (ptwo)\n");
			n = read(fd2, buf, 10);
			printf(" read %d from ptwo \n", n);
		}

	}
	return 0;
}
