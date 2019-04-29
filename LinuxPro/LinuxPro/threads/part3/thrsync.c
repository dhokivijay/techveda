#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE } Boolean;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int avail = 0;

static void *prod_start(void *arg)
{
	int cnt = *((int *)arg);
	int j;

	for (j = 0; j < cnt; j++) {
		sleep(1);
		pthread_mutex_lock(&mtx);
		avail++;
		printf("%s: Data produced\n", __func__);
		pthread_mutex_unlock(&mtx);
	}

	return NULL;
}

static void *cons_start(void *arg)
{

	int numConsumed = 0;
	Boolean done = FALSE;
	int cnt = *((int *)arg);

	for (;;) {
		pthread_mutex_lock(&mtx);
		while (avail > 0) {	/* Consume all available units */
			/* Do something with produced unit */
			numConsumed++;
			avail--;
			printf("%s: Data Consumed=%d\n", __func__, numConsumed);
			done = numConsumed >= cnt;
		}
		pthread_mutex_unlock(&mtx);
		if (done)
			break;
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t ptid, ctid;
	int ret, j;
	int totRequired;
	totRequired = 10;

	ret = pthread_create(&ptid, NULL, prod_start, &totRequired);
	if (ret != 0) {
		perror("pthread_create:");
		exit(1);
	}
	ret = pthread_create(&ctid, NULL, cons_start, &totRequired);
	if (ret != 0) {
		perror("pthread_create:");
		exit(1);
	}
	pthread_exit(NULL);
}
