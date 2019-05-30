#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <datamon.h>

struct shdata obj;

static void *writer(void *arg)
{
	int ret;
	obj_write(&obj, (int)10, (int)20);
	return NULL;
}

static void *reader(void *arg)
{
	int ret;
	obj_read(&obj);
	return NULL;
}

int main()
{
	pthread_t t1, t2;
	int ret;

	ret = obj_init(&obj);
	if (ret != 0) {
		printf("obj init failed\n");
		exit(1);
	}
	pthread_create(&t1, NULL, writer, NULL);
	pthread_create(&t2, NULL, reader, NULL);

	pthread_exit(NULL);

	return 0;
}
