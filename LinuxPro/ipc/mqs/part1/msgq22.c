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

int main(){
	int qid;
	int i;
	msg m1,m2;
	qid = msgget(KEY,IPC_CREAT);
	i = msgrcv(qid,&m1,sizeof(msg),10,0);
	printf("%s\n",m1.buffer);
	
	i = msgrcv(qid,&m2,sizeof(msg),20,0);
	printf("%s\n",m2.buffer);
	return 0;
}
