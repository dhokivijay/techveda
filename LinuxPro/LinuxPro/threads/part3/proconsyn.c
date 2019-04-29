#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE } Boolean;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int avail = 0;

static void *threadFunc(void *arg)
{
	int cnt = *((int *)arg);
	int j, ret;

	for (j = 0; j < cnt; j++) {
		/* Code to produce units */
		pthread_mutex_lock(&mtx);

		avail++; /* Let consumer know another unit is available */

		pthread_mutex_unlock(&mtx);
		ret = pthread_cond_signal(&cond); /* Wake sleeping consumer */
		if (ret != 0)
			perror("pthread_cond_signal");
	}

	pthread_exit(NULL);
}

int main()
{
	pthread_t tid;
	int ret, j;
	int totRequired;	/* Total number of units that all
				   threads will produce */
	int numConsumed = 0;	/* Total units so far consumer */
	Boolean done = FALSE;

	totRequired = 10;

	/* launch producer thread */

	ret = pthread_create(&tid, NULL, threadFunc, &totRequired);
	if (ret != 0)
		perror("pthread_create");

	/* Consumer code to process produced units */

	/* Use a polling loop to check for available units */
	for (;;) {
		pthread_mutex_lock(&mtx);
		while (avail == 0) {	/* wait for units to consume */
			ret = pthread_cond_wait(&cond, &mtx);
			/* 
			   1. unlock mutex
			   2. waits for condition to be signalled
			   3. locks mutex
			 */
			if (ret != 0)
				perror("pthread_cond_wait");
		}

		/* At this point, 'mtx' is locked... */

		while (avail > 0) {	/* Consume all available units */
			/* Do something with produced unit */
			numConsumed++;
			avail--;
			printf("numConsumed=%d\n", numConsumed);
			done = numConsumed >= totRequired;
		}

		pthread_mutex_unlock(&mtx);

		if (done)
			break;

	}

	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
}
