#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define DEATH(mess) { perror(mess); exit(errno); }

typedef struct abc {
	int a;
	int b;
} ABC;

static void *threadFunc(void *arg)
{
	ABC *p = (ABC *) arg;

	sleep(2);
	printf("%d\n", p->a);
	printf("%d\n", p->b);
//	getchar();

	pthread_exit(NULL);
}

int main()
{
	pthread_t t1;
	void *res;
	int ret;

	ABC obj;
	obj.a = 10;
	obj.b = 20;

	ret = pthread_create(&t1, NULL, threadFunc, &obj);
	if (ret != 0)
		DEATH("pthread_create");
//	pthread_join(t1,&res);
	pthread_exit(NULL);
	return 0;
}
