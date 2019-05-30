#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void cleanup(void *p)
{
	pthread_mutex_unlock(&mutex);
	printf("Cleanup: mutex released\n" );
}


void *thread_func1(void *ignored_argument)
{
	int s;
	
	pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup, NULL);
	printf("Thread 1: mutex acquired\n");
	getchar();
	sleep(10);		/* Should get canceled while we sleep */
//	pthread_mutex_unlock(&mutex);

	/* Should never get here */
	pthread_cleanup_pop(1);
	printf("Thread 1: mutex released\n");
	pthread_exit(NULL);
}

void  *thread_func2(void *ignored_argument)
{
        int s;

        sleep(5);
        /* sleep() is a cancellation point */
	pthread_mutex_lock(&mutex);
	getchar();
        printf("Thread 2: mutex acquired \n");
        pthread_mutex_unlock(&mutex);
        printf("Thread 2: mutex \n");

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

	s = pthread_create(&thr2, NULL, &thread_func2, NULL);
        if (s != 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
        }
	printf("pid %d\n",getpid());

	sleep(2);		/* Give thread a chance to get started */

	printf("\nmain(): sending cancellation request\n");

	s = pthread_cancel(thr1);
	if (s != 0) {
		perror("pthread_cancel");
                exit(EXIT_FAILURE);
	}

	/* Join with thread to see what its exit status was */

	s = pthread_join(thr1, &res);
	if (s != 0) {
		perror("pthread_join");
                exit(EXIT_FAILURE);
	}
	if (res == PTHREAD_CANCELED)
		printf("main(): thread was canceled\n");
	else
		printf("main(): thread wasn't canceled (shouldn't happen!)\n");
	
	s = pthread_join(thr2, &res);
        if (s != 0) {
                perror("pthread_join");
                exit(EXIT_FAILURE);
        }
	return 0;
}
