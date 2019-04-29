/* example showing how to create msgq.
Version : 1.0
Author : Team -C
*/

# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

# define KEY 8979

typedef struct{
	long msgtype;
	char buffer[100];
}msg;
main(){
	int qid;
	int i;
	msg m1,m2;
	qid = msgget(0,IPC_PRIVATE);
	i = msgrcv(qid,&m1,sizeof(msg),10,0);
	printf("%s",m1.buffer);
	i = msgrcv(qid,&m2,sizeof(msg),20,0);
	printf("%s",m2.buffer);
	exit(0);
}
