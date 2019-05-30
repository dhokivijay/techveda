#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	printf("pid %d\n", getpid());
	while(1)
	{
		printf("in loop\n");
		sleep(2);
	}
	return 0;
}
