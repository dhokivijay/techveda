#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct shdata {
	int a;
	int b;
};


pthread_mutex_t mtx;

struct shdata obj; 
	
static void * writer(void *arg) 
{
	pthread_mutex_lock(&mtx);
    	obj.a=10;
    	sleep(1);
    	obj.b=20;
	pthread_mutex_unlock(&mtx);
	
    return NULL;
}

static void * reader(void *arg)
{
     sleep(1);
     pthread_mutex_lock(&mtx);
     printf("obj.a = %d\n",obj.a);
     printf("obj.b = %d\n",obj.b);
     pthread_mutex_unlock(&mtx);
    return NULL;
}

int main()
{

    pthread_t t1, t2;
    int ret;

    pthread_mutex_init(&mtx, NULL);
    ret = pthread_create(&t1, NULL, writer, NULL);
    if (ret != 0)
        perror("Pthread Create :   ");

    ret = pthread_create(&t2, NULL, reader, NULL);
    if (ret != 0)
	perror("Pthread Create:    ");
	
    pthread_exit(NULL);

}
