#include <stdio.h>
#include <signal.h>

void check_blocked_sigs()
{
	int i, res,data;
	sigset_t s;
	sigprocmask(SIG_BLOCK, NULL, &s);//first param is not considered
	for (i = 1; i < 6; i++) {
		res = sigismember(&s, i);
		if (res)
			printf(" signal %d is blocked  data \n", i);
		else
			printf(" signal %d is not blocked \n", i);
	}
}

int main()
{
	sigset_t s_set;int data;
	sigemptyset(&s_set);
	sigaddset(&s_set, 1);
	sigaddset(&s_set, 4);
	
	sigprocmask(SIG_BLOCK | SIG_SETMASK, &s_set, NULL);
	check_blocked_sigs();
	sigprocmask(SIG_UNBLOCK, &s_set, NULL);
	check_blocked_sigs();
	

	return 0;
}
