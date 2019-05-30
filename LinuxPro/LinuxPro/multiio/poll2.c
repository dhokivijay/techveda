#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <poll.h>
#include <stropts.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	int fd1, fd2;
	struct pollfd pollarray[10];
	char buf[10];
	int n;

	fd1 = open("./pone", O_RDWR);
	fd2 = open("./ptwo", O_RDWR);

	/* step 1: initialize file desctiptors to be watched into 
                   pollfd instances */

	pollarray[0].fd = fd1;
	pollarray[1].fd = fd2;
	pollarray[0].events = POLLIN;
	pollarray[1].events = POLLIN;

	while (1) {
		/*step 2: setup watch */
		n = poll(pollarray, 2, 90000);
		if (n < 0) {
			perror("poll:");
			exit(1);
		}
		/* step 3: test whether fd1 is ready or not */
		if ((pollarray[0].revents & POLLIN) == 1) {
			printf(" reading from fd1 (pone)\n");
			n = read(fd1, buf, 10);
			printf(" read %d from pone \n", n);
		}
		/* test whether fd2 is ready or not */
		if ((pollarray[1].revents & POLLIN) == 1) {
			printf(" reading from fd2 (ptwo)\n");
			n = read(fd2, buf, 10);
			printf(" read %d from ptwo \n", n);
		}
	}
	return 0;		// must never reach here
}
