#include <stdio.h>
#include <linux/errno.h>
#include <pthread.h>
#include <sched.h>

void *t_routine(void *null)
{
	int policy;
        struct sched_param param;

        pthread_getschedparam(pthread_self(), &policy, &param);
        // 0 - SCHED_OTHERS     1 - SCHED_FIFO          2 - SCHED_RR
        printf("\n Current Thread Policy : %d Priority : %d\n", policy,
               param.sched_priority);


}

int main()
{
	int inherit, policy, priority, rc;
	pthread_t tcb;
	pthread_attr_t attr;
	struct sched_param param;
	
	param.sched_priority = 60;
        sched_setscheduler(0, SCHED_RR, &param);
	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_create(&tcb, &attr, t_routine, NULL);
	pthread_exit(NULL);

}
