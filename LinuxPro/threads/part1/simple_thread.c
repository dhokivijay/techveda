#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

static void *threadFunc(void *arg)
{
	char *s = (char *)arg;

	printf("%s: arg string :%s", __func__, s);

	getchar();
	return (void *)strlen(s);
}

int main()
{
	pthread_t t1;
	void *res;
	int ret;

	ret = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
	if (ret != 0) {
		perror("Thread creation failed  \n");
		exit(1);
	}
	ret = pthread_join(t1, &res);
	if (ret != 0)
		perror("pthread_join"); 

	printf("%s: Thread returned %ld\n", __func__, (long)res);
	return 0;
}
