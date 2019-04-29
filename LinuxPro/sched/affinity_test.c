#define _GNU_SOURCE

#include <sched.h>
#include <unistd.h>
#include <stdio.h>

int main()
{

	int i=0;
	int pid;
	int no_processors;
	cpu_set_t cset;

	printf("PID %d\n", getpid());

	getchar();
	no_processors = sysconf(_SC_NPROCESSORS_CONF);
        printf("System has %d processors.\n", no_processors);

	CPU_ZERO( &cset );
	CPU_SET( 0, &cset);
	CPU_SET( 1, &cset);

	if( sched_setaffinity( 0, sizeof(cset), &cset ) == -1 )
                perror("CPU Affinity...\n");

	return 0;	
	
}

