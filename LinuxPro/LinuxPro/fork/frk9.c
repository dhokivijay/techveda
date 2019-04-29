#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	int local = 2;
	pid_t childPid;

	switch (vfork()) {

	case -1:
		perror("vfork");
		exit(1);
		break;

	case 0:
		sleep(3);
		/* Even if we sleep for a while,
		   parent still is not scheduled */
		write(STDOUT_FILENO, "Child executing\n", 16);
		local *= 10;	/* This change will be seen by parent */
		break;

	default:
		/* Parent is blocked until child exits */
		write(STDOUT_FILENO, "Parent executing\n", 17);
		printf("local=%d\n", local);
		break;
	}
	exit(0);

}
