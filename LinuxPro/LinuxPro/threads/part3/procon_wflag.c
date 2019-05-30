#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef enum { FALSE, TRUE } Boolean;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static int avail = 0;
static sem_t sem;

static void *threadFunc(void *arg)
{
	int cnt = *((int *)arg);
	int j;

	for (j = 0; j < cnt; j++) {
		sleep(1);
		pthread_mutex_lock(&mtx);
		avail++;	
		pthread_mutex_unlock(&mtx);
		sem_post(&sem); // wake-up consumer 
	}

	return NULL;
}

int main()
{
	pthread_t tid;
	int ret, j;
	int totRequired;	
				   
	int numConsumed = 0;	
	Boolean done = FALSE;

	totRequired = 10;

	sem_init(&sem, 0, 0);

	ret = pthread_create(&tid, NULL, threadFunc, &totRequired);
	if (ret != 0)
		perror("pthread_create:      ");

	for (;;) {
		pthread_mutex_lock(&mtx);

		/* check if data is produced, if data is not ready yet
		   unlock mutex & wait for data	to be produced */
	
		if(avail == 0){
			pthread_mutex_unlock(&mtx);	
			/* using semaphore as conditional wait flag */
			sem_wait(&sem);
			pthread_mutex_lock(&mtx);
		}
		while (avail > 0) {	
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
}
