#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	int fd;
	char ch='A';
	fd = open("/root/temp.log",O_CREAT|O_RDWR);
	daemon(0,0);
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
		write(fd,&ch,1);	
}
