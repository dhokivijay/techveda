#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func1(void *ignored_argument)
{
	int s;
	pthread_mutex_lock(&mutex);
	printf("Thread 1 acquire mutex \n");
	for(s = 0; s < 10000; s++)
		printf("in loop\n");
	sleep(20);		
	pthread_mutex_unlock(&mutex);
	printf("Thread 1 releases mutex \n");

	/* Should never get here */

	return NULL;
}


static void *thread_func2(void *ignored_argument)
{
        int s;

        sleep(5);
        /* sleep() is a cancellation point */
	printf("Thread 2 , waiting for mutex\n");
	pthread_mutex_lock(&mutex);
        printf("Thread 2 acquire mutex \n");
        pthread_mutex_unlock(&mutex);
        printf("Thread 2 releases mutex \n");


        return NULL;
}

int main(void)
{
	pthread_t thr1,thr2;
	void *res;
	int s;

	/* Start a thread and then send it a cancellation request */

	s = pthread_create(&thr1, NULL, &thread_func1, NULL);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}


	printf("main(): sending cancellation request\n");
	sleep(1);

	s = pthread_cancel(thr1);

	s = pthread_create(&thr2, NULL, &thread_func2, NULL);
        if (s != 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
        }
	//sleep(2);		/* Give thread a chance to get started */

	/* Join with thread to see what its exit status was */
	s = pthread_join(thr1, &res);
	if (s != 0) {
		perror("pthread_join");
                exit(EXIT_FAILURE);
	}
	
	s = pthread_join(thr2, &res);
        if (s != 0) {
                perror("pthread_join");
                exit(EXIT_FAILURE);
        }

	
	exit(EXIT_SUCCESS);
}
