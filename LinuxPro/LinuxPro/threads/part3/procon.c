#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE } Boolean;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // lock protecting data

static int avail = 0;// shared data unit 

/* producer code */
static void *threadFunc(void *arg)
{
	int cnt = *((int *)arg);
	int j;

	for (j = 0; j < cnt; j++) {
		sleep(1);
		pthread_mutex_lock(&mtx);
		avail++;	
		pthread_mutex_unlock(&mtx);
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

	ret = pthread_create(&tid, NULL, threadFunc, &totRequired);
	if (ret != 0)
		perror("pthread_create:      ");

	for (;;) {

		/* acquire lock on shared data */
		pthread_mutex_lock(&mtx);

		/* check if data is produced , consume if ready */
		while (avail > 0) { 
		    numConsumed++;
		    avail--;
		    printf("%s:Data Unit Consumed=%d\n",__func__, numConsumed);
		    done = numConsumed >= totRequired;
		}
		/* unlock mutex */
		pthread_mutex_unlock(&mtx);

		if (done)
			break;

	}

	pthread_exit(NULL);
}
