/* entry point test */
# include <stdio.h>
# include <stdlib.h>

/* program entry point */
int _start()
{
	printf("%s: in start routine\n",__func__);
	xyz();
	_fini();
	exit(0);
}
/* start of functionality */
int xyz()
{
	printf("%s: begin of app functionality\n",__func__);
	test();
	return 0;
}
/* program destructor */
int _fini()
{
	printf("cleanup invoked\n");
	return 0;
}	
