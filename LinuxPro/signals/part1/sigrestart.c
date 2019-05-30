#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void handler(int signum)
{
	printf("%s:handler invoked for signal : %d\n", __func__, signum);
	sleep(1);
	printf("%s: exit from sighandler\n",__func__);
}

int main()
{
	struct sigaction act;
	int fd, ret;
	char buf[20];

	memset(&act, 0, sizeof(act));
	act.sa_flags = SA_RESTART;
	act.sa_handler = handler;
	if (sigaction(SIGINT, &act, NULL) < 0)	
	{
		perror("sigaction \n");
		exit(1);
	}

	fd = open("./myfifo", O_RDWR);
	if (fd < 0)
	{
		perror("open \n");
		return fd;
	}
	printf("%s:Descriptor created with fd:%d\n",__func__, fd);
	ret = read(fd, buf, 30);
	if (ret < 0){
		perror("read:");
		return ret;
	}
	printf("%s: no of bytes read %d :Data read %s \n",__func__, ret, buf);
	return 0;
}
