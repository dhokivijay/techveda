#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
	int ret;
	void *ptr;
	unsigned char mincore_vec[5];
	size_t page_size;
	size_t size;
	size_t page_index;

	/*get arch specific page size */
	page_size = getpagesize();
	size = page_size * 5;

	/* Allocate 20k buffer(mmap region) */
	posix_memalign(&ptr, page_size, size);

	mlock(ptr, size);	
	/* fill buffer with 0's */
//	memset(ptr,0,size);

	/* verify physical memory map */
	ret = mincore(ptr, size, mincore_vec);
	if(ret<0)
		perror("mincore");

	/* print results */
	for (page_index = 0; page_index < 5 ; page_index++) {
		if (mincore_vec[page_index] & 1) 
			printf("page %lu active\n", (unsigned long)page_index);
		else
			printf("page %lu not active\n", (unsigned long)page_index);
	}
	/* release buffer */
	munmap(ptr, size);
	return 0;
}
