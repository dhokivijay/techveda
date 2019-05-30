
/* prod_no_condvar.c

   A simple POSIX threads producer-consumer example.

*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE } Boolean;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int avail = 0;

static void * producer(void *arg)
{
    int cnt = *((int *) arg);
    int j,ret;

    for (j = 0; j < cnt; j++) {
        sleep(2);

        /* Code to produce units */

         pthread_mutex_lock(&mtx);

         avail++;/* Let consumer know another unit is available */
        pthread_mutex_unlock(&mtx);
	ret = pthread_cond_broadcast(&cond);         /* Wake sleeping consumer */
        if (ret != 0)
            perror( "pthread_cond_signal");
	avail--;
    }

    return NULL;
}


static void *consumer1(void *arg)
{
	int ret;
 /* Consumer code to process produced units */

    for (;;) {
                 pthread_mutex_lock(&mtx);
                // while(avail==0)               /* wait for units to consume */
                 //{
                        ret = pthread_cond_wait(&cond, &mtx);
                            if (ret != 0)
                                perror( "pthread_cond_wait");
                // }

                printf("signal recieved by 1\n");
                pthread_mutex_unlock(&mtx);

}
}

static void *consumer2(void *arg)
{
 /* Consumer code to process produced units */
	int ret;
	usleep(10);
for (;;) {
                 pthread_mutex_lock(&mtx);
                 //while(avail==0)               /* wait for units to consume */
                 //{
                        ret = pthread_cond_wait(&cond, &mtx);
                            if (ret != 0)
                                perror( "pthread_cond_wait");
                 //}

                printf("signal recieved by 2\n");
               // avail--;
                pthread_mutex_unlock(&mtx);

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

    ret = pthread_create(&tid, NULL, consumer2, NULL);
	        if (ret != 0)
            	 perror("pthread_create:      ");

    pthread_exit(NULL);
    exit(EXIT_SUCCESS);
}
