/* example showing how to get information of  msgq.
Version : 1.0
Author : Team -C
*/

# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

# define KEY 8979

int main(){
	int qid;
	struct msqid_ds buf;

	qid = msgget(KEY,IPC_CREAT);
	printf(" qid = %d\n", qid);

	msgctl(qid,IPC_RMID,NULL);
	return 0;
}
