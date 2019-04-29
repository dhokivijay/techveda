#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

static void *Thr_start(void *arg)
{
	char *s = (char *)arg;
	printf("%s: current thread %d: arg string :%s", __func__,(int) pthread_self(), s);
	pthread_exit(NULL);	
}

int main()
{
	pthread_t t1,t2;
	void *res;
	int ret;

	ret = pthread_create(&t1, NULL, Thr_start, "Hello Thr1\n");
	if (ret != 0)
	{
		perror("Thread creation failed  \n");
		exit(1);
	}

	ret = pthread_create(&t2, NULL, Thr_start, "Hello Thr2\n");
	if (ret != 0) 
	{
		perror("Thread creation failed  \n");
		exit(1);
	}
//	pthread_join(&t1,&t2);	
	pthread_exit(NULL);

	printf("%s: Thread returned %ld\n", __func__, (long)res);
	return 0;
}
