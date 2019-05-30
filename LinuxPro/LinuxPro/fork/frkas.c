#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define CHILD 0

int main()
{
	pid_t cPid, cPid1;
	int childstatus, i;
	int Ccount = 2;
	cPid = fork();
	if (cPid == CHILD) {
		printf(" child: My pid = %d \n", getpid());
		exit(100);
	} else {
		cPid1 = fork();
		if (cPid1 == CHILD) {
			printf(" child: My pid = %d \n", getpid());
			exit(200);
		} else {
			for(i = 0; i < Ccount; i++)
			wait(NULL);
			printf("end of parent \n");
		}
	}
	return 0;
}

/* cPid = fork();
   if(cPid  == 0)
//	code that runs in child
   else
	wait(NULL);
//	code that runs in parent
}*/
