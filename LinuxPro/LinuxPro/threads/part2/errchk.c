#include <pthread.h>
#include <stdio.h>
#include <errno.h>

pthread_mutex_t mutex;

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

 
void * t_start(void *p)
{
	if(pthread_mutex_lock(&mutex)==0)
	{
		printf(" Writer: lock acquired\n");
	if(pthread_mutex_lock(&mutex) == EDEADLK )
                        printf("\n Recursive lock attempt detected \n");
		printf(" updating commits .......\n");
		sleep(5);
		printf(" Done : exit from mutex block\n");
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);	
}

void * t1_start(void *p)
{
	sleep(1);
	if(pthread_mutex_unlock(&mutex) == EPERM)
                printf("%s:\n Unlock failed: Mutex not owned by current thread\n", __func__);


        pthread_exit(NULL);
}

int main()
{
	pthread_t t,t1;

	/* mutex Attribute object */
	pthread_mutexattr_t attr;
	int rv;

	pthread_mutexattr_init(&attr);

	/* set mutex type to ERRORCHECK */
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK);

	/* assign mutex attributes */
	pthread_mutex_init(&mutex,&attr);
	
	rv = pthread_create(&t, NULL, t_start, NULL);
	if(rv)
	    puts("Failed to create thread");

	rv = pthread_create(&t1, NULL, t1_start, NULL);
        if(rv)
             puts("Failed to create thread");

	pthread_exit(NULL);
}
