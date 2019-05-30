#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
	int fd, flags, ret;

	char template[] = "/tmp/testXXXXXX";

	fd = mkstemp(template);
	if (fd == -1) {
		perror("mkstemp");
		exit(1);
	}

	switch (fork()) {

		case -1:
			perror("fork");
			exit(1);

		case 0:
			/* Child: change file offset and status flags */
			if (lseek(fd, 1000, SEEK_SET) == -1) {
				perror("lseek");
				exit(1);
			}

			flags = fcntl(fd, F_GETFL);	/* Fetch current flags */
			if (flags == -1) {
				perror("fcntl - F_GETFL");
				exit(1);
			}
			flags |= O_APPEND;	/* Turn O_APPEND on */
			if (fcntl(fd, F_SETFL, flags) == -1) {
				perror("fcntl - F_SETFL");
				exit(1);
			}
			break;

		default:

			/* lets test if Parent can see file-Descriptor changes made by child */
			wait(NULL);
			printf("Child has exited\n");
			printf("File offset in parent: %lld\n",
					(long long)lseek(fd, 0, SEEK_CUR));
			flags = fcntl(fd, F_GETFL);
			printf("O_APPEND flag in parent is: %s\n",
					(flags & O_APPEND) ? "on" : "off");
			break;
	}
	return 0;
}
