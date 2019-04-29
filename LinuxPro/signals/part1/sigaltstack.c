#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

static void sigsegvHandler(int sig)
{
	   int j, nptrs;
           void *buffer[100];
           char **strings;

           nptrs = backtrace(buffer, 100);
           printf("%s:backtrace() returned %d addresses\n",__func__, nptrs);

           strings = backtrace_symbols(buffer, nptrs);
           if (strings == NULL) {
               perror("backtrace_symbols");
               exit(EXIT_FAILURE);
           }

           for (j = 0; j < nptrs; j++)
               printf("%s\n", strings[j]); 

           free(strings);
           _exit(EXIT_FAILURE);	/* Can't return after SIGSEGV */
}

static void overflowStack(int callNum)
{				/* A recursive function that overflows the stack */
	char a[100000];		/* Make this stack frame large */

	printf("%s: Call %4d - top of stack near %10p\n", __func__, callNum,
	       &a[0]);
	overflowStack(callNum + 1);
}

int main()
{
	stack_t sigstack;
	struct sigaction sa;
	int j;

	/* Allocate alternate stack and inform kernel of its existence */

	sigstack.ss_sp = malloc(SIGSTKSZ);
	if (sigstack.ss_sp == NULL) {
		perror("malloc \n");
		exit(errno);
	}

	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	if (sigaltstack(&sigstack, NULL) == -1) {
		perror("sigaltstack \n");
		exit(errno);
	}

	printf("%s: Alternate stack is at %10p-%p\n", __func__, sigstack.ss_sp,
	       (char *)sbrk(0) - 1);

	sa.sa_handler = sigsegvHandler;	/* Establish handler for SIGSEGV */
	sa.sa_flags = SA_ONSTACK;	/* Handler uses alternate stack */

	if (sigaction(SIGSEGV, &sa, NULL) == -1) {
		perror("sigaction \n");
		exit(errno);
	}

	overflowStack(1);
}
