#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int func(void);

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *thr_routine(void *arg)
{
	int ret;

	printf("%s:attempting mtx lock aquisition by thread:%u\n", __func__,
	       (int)pthread_self());
	ret = pthread_mutex_lock(&mtx);

	printf("%s: invoking utility routine func(): thread %u\n", __func__,
	       (int)pthread_self());
	ret = func();
	if (ret < 0) {
		printf("%s: func() call failed, aborting thread %u....\n",
		       __func__, (int)pthread_self());
		pthread_exit(NULL);
	}
	ret = pthread_mutex_unlock(&mtx);
	if (ret == 0)
		printf("%s: successfully unlocked mutex by :%u\n", __func__,
		       (int)pthread_self());
	pthread_exit(NULL);
}

int func()
{
	return -1;
}

int main()
{
	pthread_t t1, t2, t3;
	int ret;

	ret = pthread_create(&t1, NULL, thr_routine, NULL);
	if (ret != 0)
		perror("Pthread Create :");

	ret = pthread_create(&t2, NULL, thr_routine, NULL);
	if (ret != 0)
		perror("Pthread Create :");

	pthread_exit(NULL);
}
