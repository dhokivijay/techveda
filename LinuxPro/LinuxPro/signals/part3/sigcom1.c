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

//char *buf = "hello";
int main()
{
	int result = 0;
	sigset_t waitset;
	siginfo_t info;
	pid_t cpid, ppid;
	union sigval val;

	/* let's disable async handlers and enable signal queue */
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &waitset, NULL);

	cpid = fork();
	if (cpid != 0) {
		timestamp("before sigwaitinfo()");
		/* wait for signal to arrive */
		result = sigwaitinfo(&waitset, &info);
		if (result < 0)
			printf("sigwaitinfo failed : \n");
		/* got signal */
		printf
		    ("sigwaitinfo() returned for signal %d, with message %s\n",
		     info.si_signo, (char *)info.si_ptr);
		timestamp("after sigwaitinfo()");
		exit(0);
	} else {
		char *buf = "hello";
		ppid = getppid();
		val.sival_ptr = buf;
		sigqueue(ppid, SIGRTMIN, val);

		while (1) ;
	}
	return 0;
}
