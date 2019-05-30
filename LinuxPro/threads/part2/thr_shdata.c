#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define EXIT_SUCCESS 0

struct G {
	int a;
	int b;
};

struct G obj;			/* shared data object */

static void *threadFunc1(void *arg)
{
	int ret;
	obj.a = 10;
	sleep(1);
	obj.b = 30;
	return NULL;
}

static void *threadFunc2(void *arg)
{
	int ret;
	printf("obj.a = %d\n", obj.a);
	printf("obj.b = %d\n", obj.b);
	return NULL;
}

int main()
{
	pthread_t t1, t2;
	int ret;

	ret = pthread_create(&t1, NULL, threadFunc1, NULL);
	if (ret != 0)
		perror("Pthread Create :   ");

	ret = pthread_create(&t2, NULL, threadFunc2, NULL);
	if (ret != 0)
		perror("Pthread Create:    ");

	ret = pthread_join(t1, NULL);
	if (ret != 0)
		perror("Pthread Join:     ");

	ret = pthread_join(t2, NULL);
	if (ret != 0)
		perror("Pthread Join :    ");

	return 0;
}
