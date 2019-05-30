#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

static sigcounter = 0;
void handler(int signum)
{
  printf(" %s: handler invoked counter set to %d\n", __func__, ++sigcounter);
}

int main()
{

	signal(SIGINT, SIG_IGN);
	printf("%s: signal 1 now set to be ignored \n", __func__);
	getchar();

	signal(SIGINT, handler);
	printf("%s: Application specific handler registerd \n", __func__);
	getchar();

	signal(SIGINT, SIG_DFL);
	printf( "%s: Default handler enabled \n", __func__);
	getchar();
	return 0;
}
