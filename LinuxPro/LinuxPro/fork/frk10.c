#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	char buf[2];
	fd = open("./test", O_RDONLY);
	switch (vfork()) {

	case -1:
		perror("vfork");
		exit(1);
	case 0:
		read(fd, buf, 2);
                printf(" in child %c\n", buf[0]);
                printf(" in child %c\n", buf[1]);
		close(fd);
		close(STDOUT_FILENO);
		break;
	default:
		/* Parent is blocked until child exits */
       	        read(fd, buf, 2);
                printf(" in parent %c\n", buf[0]);
                printf(" in parent %c\n", buf[1]);
		write(STDOUT_FILENO, "Parent executing\n", 17);
		break;
	}

	exit(0);
}
