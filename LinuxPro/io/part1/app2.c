
int main()
{

	fd  =  open("/root/abc");
	fd1 = open("/media/xyz");
	fd2 = open("/mnt/abc");
	read(fd, buf, 20);
	read(fd1, buf, 4096);
	read(fd2, buf , 4096);
	

}
