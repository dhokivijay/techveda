#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define DEATH(mess) { perror(mess); exit(errno); }

/* our signal handlers */
static sigcounter = 0;
static struct itimerval itv, olditv;

void handler(int what)
{
	printf("We have received SIGALRM: count %d\n", ++sigcounter);

	if (sigcounter == 10) {
		itv.it_interval.tv_sec = 10;	// periodic interval 
		itv.it_interval.tv_usec = 0;

		if (setitimer(ITIMER_REAL, &itv, &olditv) == -1) {
			perror("setitimer : ");
			exit(1);
		}
	}
	if (sigcounter == 15) {
                if (setitimer(ITIMER_REAL, &olditv, NULL) == -1) {
                        perror("setitimer : ");
                        exit(1);
                }
        }

}

int main()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));

	/* Install signal handlers */
	act.sa_handler = handler;
	if (sigaction(SIGALRM, &act, NULL) < 0)
		DEATH("sigaction");

	printf("Successfully installed signal handler\n");

	/* schedule alarm to be signalled peridically at 5 seconds interval */
	itv.it_value.tv_sec = 5;	// initial event
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 5;	// periodic interval 
	itv.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
		perror("setitimer : ");
		exit(1);
	}

	/* Do something pointless, forever */
	for (;;) {
		printf("This is a pointless message.\n");
		sleep(1);
	}

	return 0;
}
