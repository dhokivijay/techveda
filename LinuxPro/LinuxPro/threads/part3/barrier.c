#include<stdio.h>
#include<pthread.h>


pthread_barrier_t barrier;

static void * t1(void *arg)
{
	printf("T1:initiating job1\n");
	printf("T1: ......\n");
	printf("T1: Done... hitting barrier \n");
	pthread_barrier_wait(&barrier);
	printf(" T1: Resume from barrier ....\n");
}

static void * t2(void *arg)
{
        printf("T2: initiating job2\n");
	sleep(3);
	printf("T2: .....\n");
	printf("T2: Done... hitting barrier\n");
        pthread_barrier_wait(&barrier);
        printf("T2: Resume from barrier ...\n");
}

static void * t3(void *arg)
{
        printf("T3: initiating job3\n");
	sleep(4);
	printf("T3: .....\n");
	printf("T3: Done... hitting barrier\n");
        pthread_barrier_wait(&barrier);
        printf("T3: Resume from barrier ....\n");
}


int main()
{
	pthread_t tcb1,tcb2,tcb3;
	pthread_barrier_init(&barrier,NULL,3);

	pthread_create(&tcb1,NULL,t1,NULL);
	pthread_create(&tcb2,NULL,t2,NULL);
	pthread_create(&tcb3,NULL,t3,NULL);
	getchar();	
	pthread_exit(NULL);
}
