/* Shows how to use Fork system call 
Version : 1.0
Author : Team -C
*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

#define CHILD 0

int main()
{
	int fd;
	char buf[2];
	pid_t pid;
	int childstatus;
	pid = fork();
	if (pid == CHILD) {
		read(fd, buf, 2);
		perror("child:");
		printf(" in child %c fd = %d\n", buf[0],fd);
		printf(" in child %c\n", buf[1]);
	//	buf[2]="vij";
	//	write(fd,buf,2);
		close(fd);
	}
	// parent
	else {
		fd = open("./test", O_RDONLY);
		wait(&childstatus);
	//	buf[2]="par";
		//write(fd,buf,2);
		read(fd, buf, 2);
		printf(" in parent %c fd =%d \n", buf[0],fd);
		printf(" in parent %c\n", buf[1]);
		close(fd);
	}
	return 0;
}
