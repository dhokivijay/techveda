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
	siginfo_t info;
	
	/* let's disable async handlers and enable signal queue */
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGALRM);
	sigprocmask(SIG_BLOCK, &waitset, NULL);

	/* schedule an alarm event */
	alarm(5);
	timestamp("before sigwaitinfo()");

	/*
	   wait for alarm to arrive 
	   step 1: block until specified signals arrive into pending list
	   step 2: when any signal mentioned in the set occurs, return with
		   all meta-data describing signal's occurence 

        */
	result = sigwaitinfo(&waitset, &info);
	if (result < 0)
		printf("sigwaitinfo failed : \n");

	/* got alarm  */
	printf("sigwaitinfo() returned for signal %d\n", info.si_signo);
	timestamp("after sigwaitinfo()");
	return 0;
}
