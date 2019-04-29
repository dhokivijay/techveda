#include <pthread.h>
#include <stdio.h>
#include <errno.h>


pthread_mutex_t mutex;

 
void * writer(void *p)
{
	if(pthread_mutex_lock(&mutex)==0)
	{
		printf(" Writer: lock acquired\n");
		if(pthread_mutex_lock(&mutex)==0)
			printf("\n Recursive lock attempt succeded \n");
		printf(" updating commits .......\n");
		sleep(2);
		printf(" Done : exit from mutex block\n");
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);	
}

void * reader(void *p)
{
	sleep(2);	
	if(pthread_mutex_lock(&mutex)==0)
	{
          printf(" Reader : lock acquired\n");
	  printf(" Reading updates ...... \n");
	  sleep(2);
	  printf(" Done: exit from mutex block \n");
	  pthread_mutex_unlock(&mutex);
        }
        pthread_exit(NULL);
}

int main ()
{
	pthread_t tcb1,tcb2;

	// Mutex Attribute object
	pthread_mutexattr_t attr;
	int rv;

	// initialise mutex attribute object 
	pthread_mutexattr_init(&attr);

	// Set mutex type to PTHREAD_MUTEX_RECURSIVE
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);
	
	rv = pthread_create(&tcb1, NULL, writer, NULL);
	if(rv)
	    puts("Failed to create thread");

	rv = pthread_create(&tcb2, NULL, reader, NULL);
        if(rv)
             puts("Failed to create thread");

	pthread_exit(NULL);
}
