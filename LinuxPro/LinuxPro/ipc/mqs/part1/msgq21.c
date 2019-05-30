# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <string.h>

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

	/* message 1 */
	m1.msgtype=10;
	strcpy(m1.buffer,"veda");
	
	/* msg 2 */
	m2.msgtype=20;
	strcpy(m2.buffer,"solutions");
	
	i = msgsnd(qid,&m1,sizeof(msg),IPC_NOWAIT);
	printf(" return value of msgsnd of As= %d\n",i);
	i = msgsnd(qid,&m2,sizeof(msg),IPC_NOWAIT);
	printf(" return value of msgsnd of Bs= %d\n",i);
	return 0;
}
