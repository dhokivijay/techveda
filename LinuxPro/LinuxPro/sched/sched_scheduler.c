#include <sched.h>
#include <stdio.h>
#include <unistd.h>

void query_policy()
{
	int policy;
        policy = sched_getscheduler(0);

        switch(policy)
        {
                case SCHED_OTHER:
                        printf("\n Fair share scheduler......\n");
                        break;
                case SCHED_RR:
                        printf("\n Round Robin scheduler......\n");
                        break;
                case SCHED_FIFO:
                        printf("\n First In-First Out scheduler......\n");
                        break;
        }
}


int main()
{
	struct sched_param param;
	param.sched_priority=60;
	query_policy();
	getchar();
	sched_setscheduler(0, SCHED_RR , &param);
	getchar();
	query_policy();
	return 0;

}
