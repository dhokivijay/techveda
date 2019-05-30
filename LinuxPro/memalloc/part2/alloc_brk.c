#include <unistd.h>
#include <stdio.h>

void *curr_brk, *def_brk, *new_brk;

int main()
{
	int ret;
	int *p;
	
	/* grab current program break address */
	curr_brk = sbrk(0);
	def_brk = curr_brk;

	printf("\n %p \n", curr_brk);
	getchar();

	/* change the location of the program break using brk 
	   (Allocate 100 bytes of heap) */
	brk(curr_brk + 100);

	/* verify change */
	new_brk = sbrk(0);
	printf("\n %p \n", new_brk);
	getchar();

 /* restore the old location of the program break using brk */
	brk(def_brk);
	curr_brk= sbrk(0);
	printf("\n %p \n",curr_brk);
	getchar();
	return 0;
}
