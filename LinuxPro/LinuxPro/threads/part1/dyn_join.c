#include <pthread.h>
#include <stdio.h>
#include <linux/errno.h>

void read_detachstate_attribute(pthread_attr_t *attr)
{
        int detachstate;

        pthread_attr_getdetachstate(attr, &detachstate);
        if(detachstate == PTHREAD_CREATE_JOINABLE)
                printf("\nIn Joinable Mode (explicit cleanup)\n");
        else
                printf("\nIn Detached Mode (auto cleanup)\n");

}

void * t_routine(void * null)
{
	pthread_attr_t attr;

        pthread_getattr_np(pthread_self(), &attr);
        read_detachstate_attribute(&attr);

	sleep(10);

	pthread_getattr_np(pthread_self(), &attr);
	read_detachstate_attribute(&attr);

	pthread_exit(NULL);
}

int main()
{
	int detach_state,rc;
	pthread_t tcb;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_create(&tcb, &attr, t_routine, NULL); 

	sleep(1);

	// explicitly detach thread
	rc = pthread_detach(tcb); 	
	
	pthread_exit(NULL);
}
