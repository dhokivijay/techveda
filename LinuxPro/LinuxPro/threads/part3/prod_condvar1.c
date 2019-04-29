#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE } Boolean;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int avail = 0;
static int data = 0;

static void *producer(void *arg)
{
	int cnt = *((int *)arg);
	int j, ret;

	for (j = 0; j < cnt; j++) {
		sleep(1);

		/* Code to produce units */

		pthread_mutex_lock(&mtx);

		avail++;	/* Let consumer know another unit is available */
		data++;
		pthread_mutex_unlock(&mtx);
		ret = pthread_cond_broadcast(&cond);	/* Wake sleeping consumer */
		if (ret != 0)
			perror("pthread_cond_signal");
	}

	return NULL;
}

static void *consumer1(void *arg)
{
	/* Consumer code to process produced units */

	int ret, numConsumed = 0;	/* Total units so far consumer */
	Boolean done = FALSE;

	int totRequired = 10;	/* Total number of units that all
				   threads will produce */

	/* Use a polling loop to check for available units */
	for (;;) {
		pthread_mutex_lock(&mtx);
		while (avail == 0) {	/* wait for units to consume */
			ret = pthread_cond_wait(&cond, &mtx);
			if (ret != 0)
				perror("pthread_cond_wait");
		}

		numConsumed++;
		avail--;
		printf("unit Consumed by consumer1=%d\n", data);
		done = numConsumed >= totRequired;
		pthread_mutex_unlock(&mtx);
		if (done)
			break;
	}
}

int main()
{
	pthread_t tid;
	int ret, j;

	int totRequired = 10;

	/* launch producer thread */
	ret = pthread_create(&tid, NULL, producer, &totRequired);
	if (ret != 0)
		perror("pthread_create:      ");

	/* launch consumer threads */
	ret = pthread_create(&tid, NULL, consumer1, NULL);
	if (ret != 0)
		perror("pthread_create:      ");

	ret = pthread_create(&tid, NULL, consumer1, NULL);
	if (ret != 0)
		perror("pthread_create:      ");

	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
}
