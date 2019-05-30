/* example showing how to create msgq.
Version : 1.0
Author : Team -C
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 0

typedef struct {
	long msgtype;
	char buffer[100];
} msg;
int main()
{
	int qid;
	pid_t pid;
	int i;
	msg m1, m2;
	qid = msgget(KEY, IPC_PRIVATE);
	m1.msgtype = 10;
	for (i = 0; i < 100; i++)
		m1.buffer[i] = 'a';
	m2.msgtype = 20;
	for (i = 0; i < 100; i++)
		m2.buffer[i] = 'b';
	i = msgsnd(qid, &m1, sizeof(msg), IPC_NOWAIT);
	printf(" return value of msgsnd of As= %d\n", i);
	i = msgsnd(qid, &m2, sizeof(msg), IPC_NOWAIT);
	printf(" return value of msgsnd of Bs= %d\n", i);
	pid = fork();
	if (pid == 0) {
		i = msgrcv(qid, &m1, sizeof(msg), 10, 0);
		printf("%s", m1.buffer);
		i = msgrcv(qid, &m2, sizeof(msg), 20, 0);
		printf("%s", m2.buffer);
		exit(0);
	}
	exit(0);
}
