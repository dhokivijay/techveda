#include <stdio.h>
#include <linux/errno.h>
#include <pthread.h>

void * t_routine(void *null)
{
	int policy, i;
        struct sched_param param;
	
	for(i = 0; i < 100; i++)
	{
	pthread_getschedparam(pthread_self(),&policy,&param);
	// 0 - SCHED_OTHERS     1 - SCHED_FIFO          2 - SCHED_RR
 printf("\n Current Thread loop count :%d Policy : %d Priority : %d ", i , policy,param.sched_priority);
	}

}

int main ()
{
	int inherit,policy,priority,rc;
	pthread_t tcb;
	pthread_attr_t attr;
	struct sched_param param;

	pthread_attr_init(&attr);

        /* switch off sched inheritence from parent */	
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    
       /* Now assign Sched policy and priority */
	policy = SCHED_FIFO;
	pthread_attr_setschedpolicy(&attr, policy);
	
        param.sched_priority = 10;
        pthread_attr_setschedparam(&attr, &param);

	/* create thread with choosen attribs */
        pthread_create(&tcb, &attr, t_routine, NULL);

	policy = SCHED_RR;
	pthread_attr_setschedpolicy(&attr, policy);

        param.sched_priority = 10;
        pthread_attr_setschedparam(&attr, &param);

        pthread_create(&tcb, &attr, t_routine, NULL);

	/* destroy attribute object */
	pthread_attr_destroy(&attr);


	pthread_exit(NULL);

}


