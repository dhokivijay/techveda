# include <unistd.h>
# include <sys/io.h>

void setval(int what,unsigned val){
	outb(what,0x70);
	outb(val,0x71);
}
int main(){
	/* get the permission to access rtc ports */
	int res,a,b;
	res=ioperm(0x70,0x04,1);/* we can access ports 70,71,72,73*/
	printf("res =%d \n",res);

	setval(0x07,10);

	setval(0x08, 8);

	setval(0x09, 14);
	return 0;
}
