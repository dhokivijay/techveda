/* entry point test */
# include <stdio.h>
# include <stdlib.h>

int xyz(void);
/* run on exit */
void _fini()
{
	printf("cleanup invoked\n");
}	


/* program entry point */
void _start()
{
	printf("in start routine\n");
	atexit(_fini);
	exit(xyz()); 

}
/* start of functionality */
int xyz()
{
	printf("begin of app functionality\n");
	return 0;
}
