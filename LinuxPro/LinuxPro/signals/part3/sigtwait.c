#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

void timestamp(char *str)
{
	time_t t;
	time(&t);
	printf("The time %s is %s\n", str, ctime(&t));
}

int main()
{
	int result = 0;
	sigset_t waitset;
	struct timespec timeout;
	siginfo_t info;
	
	/* let's disable async handlers and enable signal queue */
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGALRM);
	sigprocmask(SIG_BLOCK, &waitset, NULL);

	/* schedule an alarm event */
	alarm(5);
	timestamp("before sigwaitinfo()");

	/* wait for alarm to arrive */
	timeout.tv_sec = 6;
	timeout.tv_nsec = 0;
	result = sigtimedwait(&waitset, &info, &timeout);
	if (result < 0)
	{
		perror("Timeout Reached: ");
		exit(1);
	}

	/* got alarm  */
	printf("sigtimedwait() returned for signal %d\n", info.si_signo);
	timestamp("after sigtimedwait()");
	return 0;
}
