#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
        int fd, i;
        int val;

        unsigned int size;
        char c;

        fd = open( "/dev/char_dev_misc", O_RDWR );
        if( fd < 0 ) {
                printf("\n\nDevice could not be opened\n\n");
                return 1;
        }
        printf("Device opened with ID [%d]\n", fd);


	while(1){
        if(read( fd,&val,1) <0)
			continue;
        printf("reading character: %c\n", val);
	}
        /* Close the device */
        close(fd);

        exit(0);
}


