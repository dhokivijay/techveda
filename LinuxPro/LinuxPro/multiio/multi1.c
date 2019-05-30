# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdlib.h>

int main()
{

	int fd1,fd2;
	char buf[10];
	int n;

	/* pipe files */
	fd1 = open("./pone",O_RDWR);
	fd2 = open("./ptwo",O_RDWR);

	printf(" Trying to read from pone\n");
	n = read (fd1,buf,10);
	printf(" read %d no of bytes from pone \n",n);

	printf(" Trying to read from ptwo\n");
	n = read (fd2,buf,10);
	printf(" read %d no of bytes from ptwo \n",n);
	return 0;
}










