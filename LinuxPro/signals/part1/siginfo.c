#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEATH(mess) { perror(mess); exit(errno); }

struct my_s {
	int x;
	char s[32];
};

/* our signal handlers */

void sig_act(int sig, siginfo_t * si, void *a)
{
	struct my_s *s = si->si_ptr;
	printf("in handler pid=%d, SIGNAL = %d\n", getpid(), sig);

	printf("si_signo = \t%d\n", si->si_signo);
	printf("si_code = \t%d\n", si->si_code);
	printf("si_pid = \t%d\n", si->si_pid);
	printf("si_uid = \t%d\n", si->si_uid);
	printf("si_ptr.x = \t%d\n", s->x);
	printf("si_ptr.s = \t%s\n", s->s);
}

int main()
{
	struct sigaction act;
	struct my_s s;
	int j, sig = SIGALRM;
	union sigval sv;

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = sig_act;
	act.sa_flags = SA_SIGINFO;
	if (sigaction(sig, &act, NULL) < 0)
		DEATH("sigaction");

	printf("pid=%d Successfully installed signal handler for signal=%d\n",
	       getpid(), sig);

	for (j = 0; j < 3; j++) {
		printf("This is a pointless message\n");
		s.x = j * 100;
		strcpy(s.s, "hello");
		sv.sival_ptr = &s;
		printf("sigqueue returns %d\n", sigqueue(getpid(), sig, sv));
		sleep(1);
	}
	return 0;
}
