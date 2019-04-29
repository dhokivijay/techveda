/* test_char_driver.c
** Team: Veda
** Version: 1.0
*/

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
	
	fd = open( "/dev/storage0", O_RDWR );
	if(fd < 0) {
		printf("failed acquiring file descriptor return status %d\n",fd);
		return fd;
	}
	
	/* write the contents of my buffer into the device */
	ret = write( fd, my_buf, 12 );

	close(fd);
}
	
