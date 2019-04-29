#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	int fd, i;
	ssize_t ret;
	char my_buf[12]="Hello world";
	
	fd = open( "/dev/vDevMisc", O_RDWR );
	if(fd<0)
	{
		printf("failed acquiring file descriptor return status %d\n",fd);
		return -1;
	}

	
	/* write the contents of my buffer into the device */
	ret = write( fd, my_buf, 12 );
	ret = read(fd,my_buf,3000);
	if(ret<0)
		printf("read operation failed with return status %ld\n",ret);
	close(fd);
}
	
