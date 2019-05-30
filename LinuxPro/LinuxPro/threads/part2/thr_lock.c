#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *thread1(void *arg)
{
	int ret;
	
	ret = pthread_mutex_lock(&mtx);
	if(ret == 0)
	 printf("%s: successfully locked mutex by thread:%u\n",__func__,(int)pthread_self());
	sleep(10);
	printf("%s:lock held by %u\n",__func__, (int)pthread_self());
	sleep(10);
	printf("%s:inside critical section %u\n", __func__,(int) pthread_self());
	ret = pthread_mutex_unlock(&mtx);
	if( ret == 0)
		printf("%s: successfully unlocked mutex by :%u\n", __func__,(int)pthread_self());
	pthread_exit(NULL);
}

void *thread2(void *arg)
{
	int ret;
	pthread_yield();
	ret = pthread_mutex_unlock(&mtx);
	if(ret == 0)
		printf("%s: successfully unlocked mutex\n",__func__);

	pthread_exit(NULL);
}

int main()
{
	pthread_t t1, t2, t3;
	int ret;

	ret = pthread_create(&t1, NULL, thread1, NULL);
	if (ret != 0)
		perror("Pthread Create :");

	ret = pthread_create(&t2, NULL, thread1, NULL);
	if (ret != 0)
		perror("Pthread Create :");

	ret = pthread_create(&t3, NULL, thread2, NULL);
	if (ret != 0)
		perror("Pthread Create:");

	pthread_exit(NULL);
}
