#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

static int func(int arg)
{
	int j;

	for (j = 0; j < 0x100; j++)
		if (malloc(0x8000) == NULL) {
			perror("malloc");
			exit(1);
		}
	printf("Program break in child:  %10p\n", sbrk(0));

	return arg;
}

int main(void)
{
	pid_t CPid;
	int status, arg;

	printf("Program break in parent: %10p\n", sbrk(0));

	switch (fork()) {

	case -1:
		perror("fork");
		exit(1);

	case 0:
		/* Child calls func() and uses return value as exit 
		   status */
		exit(func(arg));

	default:

		/* Parent waits for child to terminate. It can determine the
		   result of func() by inspecting 'status' */

		if (wait(&status) == -1) {
			perror("wait");
			exit(1);
		}

		printf("Program break in parent: %10p\n", sbrk(0));
	}
	return 0;
}
