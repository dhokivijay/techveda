#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void *Thr1_start(void *);
void *Thr2_start(void *);

pthread_t t1,t2;

int main()
{
	void *res;
	int ret;

	ret = pthread_create(&t1, NULL, Thr1_start, "Hello Thr1\n");
	if (ret != 0) {
		perror("Thread creation failed  \n");
		exit(1);
	}

	ret = pthread_create(&t2, NULL, Thr2_start, "Hello Thr2\n");
	if (ret != 0) {
		perror("Thread creation failed  \n");
		exit(1);
	}
//	pthread_join(t1,&res);
//	printf("the r1 is %d\n",(int *)res);
	pthread_exit(NULL);

	printf("%s: Thread returned %ld\n", __func__, (long)res);
	return 0;
}

void *Thr1_start(void *arg)
{
	
	char *s = (char *)arg;
	printf("%s: current thread %u: arg string :%s", __func__,(int) pthread_self(), s);
//	pthread_join(t2, NULL);
	pthread_exit(NULL);	
//	pthread_exit(11);	
}


void *Thr2_start(void *arg)
{
	char *s = (char *)arg;
	void *res;
	pthread_join(t1, NULL);
//	pthread_join(t1,&res);
	//printf("the result of t1 %d\n",(int *)res);
	printf("%s: current thread %u: arg string :%s", __func__,(int) pthread_self(), s);
}
