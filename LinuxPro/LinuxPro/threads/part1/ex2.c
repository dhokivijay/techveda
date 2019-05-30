
/* detached_attrib.c

   An example of the use of POSIX thread attributes (pthread_attr_t):
   creating a detached thread.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define DEATH(mess) { perror(mess); exit(errno); }

int *ret;
static void *joinablethread(void *x)
{
	int i = 0, tid;
	tid = pthread_self();
	ret = (int *)x;
	for (i = 0; i < 10; i++) {
		printf(" iam in thread %d\n", tid);
		sleep(5);
	}
	return &ret;
}

static void *detachedthread(void *x)
{
	int i = 0, tid;

	tid = pthread_self();
	for (i = 0; i < 10; i++) {
		printf(" iam in thread %d\n", tid);
		sleep(10);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t tcb, tcb1;
	pthread_attr_t attr;
	int ret;
	void *res;

	ret = pthread_attr_init(&attr);	/* Assigns default values */
	if (ret != 0)
		perror("pthread_attr_init");

	/* assign detached state to PTHREAD_CREATE_DECTACHED */
	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0)
		perror("pthread_attr_getdetachstate");

	ret = pthread_create(&tcb, NULL, joinablethread, (void *)1);
	if (ret != 0)
		DEATH("pthread_create");

	ret = pthread_create(&tcb1, &attr, detachedthread, (void *)1);
	if (ret != 0)
		DEATH("pthread_create");

	ret = pthread_join(tcb1, &res);
	if (ret != 0)
		perror("pthread_join failed as expected");
	printf("Joinable Thread returned %ld\n", (long)res);

	pthread_exit(NULL);
}
