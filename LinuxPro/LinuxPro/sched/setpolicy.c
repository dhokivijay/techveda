#include <sched.h>
#include <stdio.h>
#include <unistd.h>

void query_policy()
{
	struct sched_param param;
	int policy;
	policy = sched_getscheduler(0);
	sched_getparam(0, &param);

	switch (policy) {
	case SCHED_OTHER:
		printf("%s: current policy in force :SCHED_OTHER & prio: %d\n",
		       __func__, param.sched_priority);
		break;
	case SCHED_RR:
		printf("%s: current policy in force :SCHED_RR & prio %d\n",
		       __func__, param.sched_priority);
		break;
	case SCHED_FIFO:
		printf("%s: current policy in force :SCHED_FIFO & prio %d\n",
		       __func__, param.sched_priority);
		break;
	}
}

int main()
{
	struct sched_param param;
	query_policy();
	param.sched_priority = 60;
	sched_setscheduler(0, SCHED_RR, &param);
	getchar();
	query_policy();
	return 0;
}
