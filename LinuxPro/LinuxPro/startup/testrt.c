/* entry point test */
# include <stdio.h>
# include <stdlib.h>

/* run on exit */
void _fini()
{
	printf("cleanup invoked\n");
}	


int main()
{
	printf("begin of app functionality\n");
	return 0;
}
