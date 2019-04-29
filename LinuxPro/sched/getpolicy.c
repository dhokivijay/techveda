#include <sched.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	int policy;
	policy = sched_getscheduler(0);
	switch (policy) {
	case SCHED_OTHER:
		printf("%s: Current policy in force: SCHED_OTHER\n", __func__);
		break;
	case SCHED_RR:
		printf("%s: Current policy in force: SCHED_RR\n", __func__);
		break;
	case SCHED_FIFO:
		printf("%s: Current policy in force: SCHED_FIFO\n", __func__);
		break;
	}
	return 0;
}
