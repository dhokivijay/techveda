#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <malloc.h>
void  *alloc_noswap(int size)
{
	void *ptr;

	ptr = malloc(size);
	if (ptr == NULL)
		return NULL;

	return ptr;
}

void free_noswap(void *ptr, int size)
{
	/* free the memory */
	free(ptr);
}

int main()
{
	int *p,ret;

	/* lock all pages mapped to the process */
	ret = mlockall(MCL_CURRENT | MCL_FUTURE);
	if(ret<0)
		perror("mlockall ");
	
	p = (int *)alloc_noswap(100);

	free_noswap(p,100);
	
	/* release all pages mapped to the process */
	munlockall();

}
