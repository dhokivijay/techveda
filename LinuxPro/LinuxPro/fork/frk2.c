#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define CHILD 0

int main()
{
	pid_t cPid;
	int childstatus;
	int k = 10;

	cPid = fork();
	if (cPid == CHILD) {
		printf(" child: My pid = %d \n", getpid());
		printf(" end of child k = %d\n", k);
		k = 44;//cow 
		printf(" end of child k = %d\n", k);
		printf(" end of child address of k  = %x\n", &k);
		getchar();
		exit(100);
	} else {
		wait(&childstatus);
		printf("end of parent k = %d\n", k);
		printf(" end of parent address of k  = %x\n", &k);
		getchar();
	}
	return 0;
}

/* 

   cPid = fork();
   if(cPid  == 0)
	code that runs in child
   else
	wait(NULL); // synchronous destruction 
	code that runs in parent


wait(int *childstatus)
{
	step 1: suspend caller until child terminates
	step 2: gather exit value of child and return that as out param
		(Cause of child termination can be discovered through 
		 this value)
	step 3: instruct process manager to destroy defunct child
}
*/
