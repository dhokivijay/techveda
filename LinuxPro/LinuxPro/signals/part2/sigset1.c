#include <stdio.h>
#include <signal.h>

void sighand(int no)
{
	printf(" I am in sighandler %d \n", no);
}

int main()
{
	sigset_t s_set;
	signal(62, sighand);
	signal(4, sighand);

	printf("pid %d\n", getpid());
	sigemptyset(&s_set);
	sigaddset(&s_set, 62);
	sigaddset(&s_set, 4);

	sigprocmask(SIG_BLOCK | SIG_SETMASK, &s_set, NULL);

	/* code runs with signals specified in mask disabled */
	printf(" %s: Send me signal one and see the effect now \n",__func__);
	getchar();

	sigprocmask(SIG_UNBLOCK, &s_set, NULL);
	printf(" %s:signals are unblocked \n",__func__);
	while (1);
	return 0;// should never reach here
}
