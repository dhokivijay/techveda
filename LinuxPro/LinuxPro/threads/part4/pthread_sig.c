/* This program illustrates the use of signals in threads.

   Three threads including the main thread.
   main thread
     a. Set up a signal mask to block all signals.
     b. Set up signal handlers for SIGINT and SIGUSR1.
     c. Create thread_1, detached.
     d. Create thread_2, nondetached.
     e. Send SIGINT & SIGUSR1 to thread_1.
     f. Quit.

   thread_1
     a. Unblock all to embrace all signals.
     b. Wait for signals.
     c. Send SIGINT and SIGUSR1 to thread_2
     d. Wait for thread_2 to terminate
     e. Print thread_2 return status.
     f. Quit

   thread_2
     a. Unblock SIGUSR1 -- all others blocked due to inheritance.
     b. Wait for signals.
     c. Quit

   To compile:  gcc  pthread_sig.c -lpthread 
*/

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_t tid2;
static void int_handler(int signo), usr1_handler(int signo);

void millisleep(int milliseconds)
{
	usleep(milliseconds * 1000);
}

int main()
{

	pthread_t tid1;
	pthread_attr_t attr_obj;	/* a thread attribute variable */
	void *thread_1(void *), *thread_2(void *);
	sigset_t sigmask;
	struct sigaction action;

	/* set up signal mask to block all in main thread */
	sigfillset(&sigmask);	/* to turn on all bits */
	pthread_sigmask(SIG_BLOCK, &sigmask, NULL);

	/* set up signal handlers for SIGINT & SIGUSR1 */
	action.sa_flags = 0;
	action.sa_handler = int_handler;
	sigaction(SIGINT, &action, NULL);

	action.sa_handler = usr1_handler;
	sigaction(SIGUSR1, &action, NULL);

	pthread_attr_init(&attr_obj);	/* init it to default */
	pthread_attr_setdetachstate(&attr_obj, PTHREAD_CREATE_DETACHED);

	pthread_create(&tid1, &attr_obj, thread_1, (void *)NULL);
	printf("%s: TID(%u) created\n", __func__, (unsigned int)tid1);

	pthread_attr_setdetachstate(&attr_obj, PTHREAD_CREATE_JOINABLE);
	pthread_create(&tid2, &attr_obj, thread_2, (void *)NULL);
	printf("%s: TID(%u) created\n", __func__, (unsigned int)tid2);

	millisleep(1000);	/* let's put main to wait */

	printf("%s: main(%u) sending SIGINT to TID(%u)\n", __func__,
	       (unsigned int)pthread_self(), (unsigned int)tid1);
	pthread_kill(tid1, SIGINT);

	printf("%s: main(%u) sending SIGUSR1 to TID(%u)\n", __func__,
	       (unsigned int)pthread_self(), (unsigned int)tid1);
	pthread_kill(tid1, SIGUSR1);

	printf("%s: main(%u) is terminating\n", __func__,
	       (unsigned int)pthread_self());
	pthread_exit(NULL);	/* will not terminate process */

}				/* end of main */

void *thread_1(void *dummy)
{
	int sig, status, *status_ptr = &status;

	sigset_t sigmask;
	sigfillset(&sigmask);
	pthread_sigmask(SIG_UNBLOCK, &sigmask, NULL);
	sigwait(&sigmask, &sig);
	switch (sig) {
	case SIGINT:
		int_handler(sig);
		break;
	default:
		break;
	}
	printf("%s: TID(%u) sending SIGINT to %u\n", __func__,
	       (unsigned int)pthread_self(), (unsigned int)tid2);
	pthread_kill(tid2, SIGINT);
	printf("%s: TID(%u) sending SIGUSR1 to %u\n", __func__,
	       (unsigned int)pthread_self(), (unsigned int)tid2);
	pthread_kill(tid2, SIGUSR1);

	pthread_join(tid2, (void **)status_ptr);
	printf("%s: TID(%u) exit status = %d\n", __func__, (unsigned int)tid2,
	       status);

	printf("%s: TID(%u) is terminating\n", __func__,
	       (unsigned int)pthread_self());
	pthread_exit(NULL);	/* calling thread will terminate */
}				/* thread_1 */

void *thread_2(void *dummy)
{
	int sig;
	sigset_t sigmask;

	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &sigmask, NULL);
	sigwait(&sigmask, &sig);
	switch (sig) {
	case SIGUSR1:
		usr1_handler(sig);
		break;
	default:
		break;
	}
	printf("%s: TID(%u) is terminating\n", __func__,
	       (unsigned int)pthread_self());
	pthread_exit(NULL);	/* calling thread will terminate */
}				/* thread_2 */

static void int_handler(int dummy)
{
	printf("%s: SIGINT received by TID(%u)\n", __func__,
	       (unsigned int)pthread_self());
}				/* int_handler */

static void usr1_handler(int dummy)
{
	printf("%s: SIGUSR1 received by TID(%u)\n", __func__,
	       (unsigned int)pthread_self());
}				/* usr1_handler */
