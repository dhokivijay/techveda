#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/signalfd.h>

int main()
{
	int fd1, fd2, sfd;
	fd_set read_set, write_set, except_set;
	char buf[100];
	struct itimerval itv;
	struct timeval timeout;
	sigset_t waitset;
	struct signalfd_siginfo info;
	int n;

	fd1 = open("./pone", O_RDWR);
	fd2 = open("./ptwo", O_RDWR);

	sigemptyset(&waitset);
	sigaddset(&waitset, SIGALRM);
	sigprocmask(SIG_BLOCK, &waitset, NULL);
	sfd = signalfd(-1, &waitset, 0);

	itv.it_value.tv_sec = 1;	// initial event
	itv.it_value.tv_usec = 10;
	itv.it_interval.tv_sec = 5;	// periodic interval 
	itv.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
		perror("setitimer : ");
		exit(1);
	}

	FD_ZERO(&read_set);
	FD_SET(fd1, &read_set);
	FD_SET(fd2, &read_set);
	FD_SET(sfd, &read_set);

	while (1) {

		timeout.tv_sec = 90;
		timeout.tv_usec = 0;
		n = select(FD_SETSIZE, &read_set, NULL, NULL, &timeout);
		if (n < 0) {
			perror("select ");
			exit(1);
		}
		printf("Time left %ld\n", timeout.tv_sec);

		// test whether fd1 is ready or not
		if (FD_ISSET(fd1, &read_set)) {
			printf(" reading from fd1 (pone)\n");
			n = read(fd1, buf, 100);
			printf(" read %d from pone  %s\n", n, buf);
			write(1, buf, 100);
		}
		// test whether fd2 is ready or not
		if (FD_ISSET(fd2, &read_set)) {
			printf(" reading from fd2 (ptwo)\n");
			n = read(fd2, buf, 10);
			printf(" read %d from ptwo \n", n);
		}
		if (FD_ISSET(sfd, &read_set)) {
			read(sfd, &info, sizeof(struct signalfd_siginfo));
			printf("got signal %d \n", info.ssi_signo);
		}

	}
	return 0;
}
