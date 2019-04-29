#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

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
	struct itimerval timer;
	siginfo_t info;

	/* let's disable async handlers and enable signal queue */
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGALRM);
	sigprocmask(SIG_BLOCK, &waitset, NULL);

	/* schedule periodic alarm  */
	timer.it_value.tv_sec = 5;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 5;
	timer.it_interval.tv_usec = 0;

	setitimer(ITIMER_REAL, &timer, NULL);

	timestamp("before sigwaitinfo()");

	while (1) {

		/* wait for alarm to arrive */
		result = sigwaitinfo(&waitset, &info);
		if (result < 0) {
			perror("Timeout Reached: ");
			exit(1);
		}

		/* got alarm  */
		printf("sigtimedwait() returned for signal %d\n",
		       info.si_signo);
		timestamp("after sigtimedwait()");
	}
	return 0;
}
