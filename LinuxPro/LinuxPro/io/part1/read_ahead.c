#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
	char buf[30];
	int fd, rev;

	if (fd = open("./linux.mp4", O_RDWR) < 0) {
		printf("\n open to fail");
		exit(1);
	}

/*	posix_fadvise(fd, 0 , 20, POSIX_FADV_NORMAL);	 Ok with default read_ahead */
	rev = posix_fadvise(fd, 0, 20, POSIX_FADV_SEQUENTIAL);	// Enable full read_ahead 
	if (rev != 0)
		perror("posix_fadvise : ");
/*	posix_fadvise(fd, 0 , 20, POSIX_FADV_RANDOM);  Disable read_ahead */

	if (read(fd, buf, 1000) < 0) {
		printf("\n read error");
		exit(1);
	}
	printf("\n The read  msg : %s", buf);
	close(fd);
	exit(EXIT_SUCCESS);

}
