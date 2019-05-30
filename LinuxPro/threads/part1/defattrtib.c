#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>



/* Reads Stack attributes  */
void read_stack_attributes(pthread_attr_t *attr)
{
        void *getStkAddr;
        size_t getStkSize;

        pthread_attr_getstack(attr, &getStkAddr,&getStkSize);
        printf("\nDefault Stack Size = %ld \n",getStkSize);
}

/* Reads Detach State attribute */
void read_detachstate_attribute(pthread_attr_t *attr)
{
        int detachstate;

	pthread_attr_getdetachstate(attr, &detachstate);
	if(detachstate == PTHREAD_CREATE_JOINABLE)
		printf("\nDefault cleanup mode :Joinable (explicit cleanup)\n");
	else
		printf("\nDefault cleanup mode :Detached (auto cleanup)\n");
		
}

/* Reads guard size attribute */
void read_guard_attribute(pthread_attr_t *attr)
{
        size_t guardsize;
	pthread_attr_getguardsize(attr,&guardsize);
        printf("\nDefault stack Guard Size = %ld\n", guardsize);
}

/* Read  schedpolicy attribute */
void read_schedpolicy_attribute(pthread_attr_t *attr)
{
        int policy;
        pthread_attr_getschedpolicy(attr,&policy);

	if(policy == SCHED_OTHER )
		printf("\nSched policy : SCHED_OTHER(preemptive fair-share scheduler)\n");
	else if(policy == SCHED_RR )
		printf("\nSched policy : SCHED_RR(static priority preemptive round robin)\n");
	else if(policy == SCHED_FIFO )
		printf("\nSched plicy :SCHED_FIFO( static priority preemptive cooperative scheduler\n");
}


void* t_routine(void* param)
{
	pthread_attr_t attr;
	pthread_getattr_np(pthread_self(), &attr);	

	printf("\nDefault Attrib Dump \n");
	read_stack_attributes(&attr);

	read_guard_attribute(&attr);

	read_detachstate_attribute(&attr);

	read_schedpolicy_attribute(&attr);
        return 0;
}




int main(int argc, char *argv[])
{
        pthread_t t1;

        pthread_create(&t1, NULL, t_routine, NULL);

	pthread_join(t1,NULL);
        return 0;
}
