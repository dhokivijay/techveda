#include <pthread.h>
#include <stdio.h>

typedef struct {

	int global;
	pthread_spinlock_t spin ; 
}glob_t;


void * thread1(void *p)
{
	
        printf("TH1: Getting lock ....... \n");
	if(pthread_spin_lock(&spin)!=0)
		printf("\n Error :: Spinlock Failed \n");
	else
	{
		printf("TH1:  Acquired spinlock, doing work ....... \n");
		sleep(2);
		pthread_spin_unlock(&spin);
		printf("TH1: Releasing spinlock\n");
	}
	
	pthread_exit(NULL);
}


void * thread2(void *p)
{
	
        printf("TH2: Getting lock ....... \n");
	if(pthread_spin_lock(&spin)!=0)
                printf("\n Error :: Spinlock Failed \n");
        else
        {
                printf("TH2:  Acquired spinlock \n  ....... \n");
		printf("TH2:  Releasing lock\n");
                pthread_spin_unlock(&spin);
        }

        pthread_exit(NULL);

}

int main ()
{
	pthread_t tcb1,tcb2;
	int rv;

	pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE /* PTHREAD_PROCESS_SHARED*/);

	rv = pthread_create(&tcb1, NULL, thread1, NULL);
	if(rv)
		puts("Failed to create thread");

	rv = pthread_create(&tcb2, NULL, thread2, NULL);
        if(rv)
                puts("Failed to create thread");

	pthread_join(tcb1,NULL);
	pthread_join(tcb2,NULL);
	puts(" Exit Main");
	return 0;
}
