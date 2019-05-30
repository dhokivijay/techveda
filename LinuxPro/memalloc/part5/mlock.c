#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <malloc.h>
void  *alloc_noswap(int size)
{
	void *ptr;

	/* allocate some memory */
	ptr = malloc(size);
	if (ptr == NULL)
		return NULL;

	/* lock this buffer into RAM */
	if (mlock(ptr, size)) {
		free(ptr);
		return NULL;
	}
	return ptr;
}

void free_noswap(void *ptr, int size)
{
	/* unlock the address range */
	munlock(ptr, size);

	/* free the memory */
	free(ptr);
}

int main()
{
	int *p;

	p = (int *)alloc_noswap(100);

	free_noswap(p,100);

}
