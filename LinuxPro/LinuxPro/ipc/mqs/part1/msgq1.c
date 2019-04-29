# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

# define KEY 8979

int main(){

	int qid;
	qid = msgget(KEY,IPC_CREAT);

	printf(" qid = %d\n", qid);
	getchar();
	msgctl(qid,IPC_RMID,NULL);
	return 0;
}
