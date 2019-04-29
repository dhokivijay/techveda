#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void func(void);

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *thread1(void *arg)
{
	int ret;

	ret = pthread_mutex_lock(&mtx);
	if (ret == 0)
		printf("%s: successfully locked mutex by thread:%u\n", __func__,
		       (int)pthread_self());
	printf("%s: invoking utility routine func()\n",__func__);
	func();
	printf("%s:lock held by %u\n", __func__, (int)pthread_self());
	ret = pthread_mutex_unlock(&mtx);
	if (ret == 0)
		printf("%s: successfully unlocked mutex by :%u\n", __func__,
		       (int)pthread_self());
	pthread_exit(NULL);
}

void *thread2(void *arg)
{
	int ret;
	sleep(2);
	pthread_exit(NULL);
}

void func()
{
	int ret;
	ret = pthread_mutex_lock(&mtx);
	if (ret == 0)
		printf("%s: successfully locked mutex by thread:%u\n", __func__,
		       (int)pthread_self());
	ret = pthread_mutex_unlock(&mtx);
	if (ret == 0)
		printf("%s: successfully unlocked mutex by :%u\n", __func__,
		       (int)pthread_self());
	pthread_exit(NULL);
}

int main()
{
	pthread_t t1, t2;
	int ret;

	ret = pthread_create(&t1, NULL, thread1, NULL);
	if (ret != 0)
		perror("pthread create :\n");

	ret = pthread_create(&t2, NULL, thread2, NULL);
	if (ret != 0)
		perror("pthread create:\n");

	pthread_exit(NULL);
}
