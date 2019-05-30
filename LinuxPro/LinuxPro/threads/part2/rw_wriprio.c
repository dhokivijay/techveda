#include <pthread.h>
#include <stdio.h>
#include <pthread_seq.h>

pthread_seq_t seqlck;
int shared = 0;

void *writer(void *arg)
{
	pthread_seq_wrlock_np(&seqlck);
	sleep(1);
	shared++;
	pthread_seq_wrunlock_np(&seqlck);
	pthread_exit(NULL);
}

void *reader(void *arg)
{
	int seqcount;
	do {
		seqcount = pthread_seq_rdlock_np(&seqlck);
		printf("READER: shared data %d\n", shared);
		sleep(3);
	} while (seqcount != pthread_seq_rdunlock_np(&seqlck));
	pthread_exit(NULL);
}

int main()
{
	pthread_t tcb1, tcb2, tcb3;
	int rv;

	pthread_seq_init_np(&seqlck);

	rv = pthread_create(&tcb1, NULL, writer, NULL);
	if (rv)
		puts("Failed to create thread");

	rv = pthread_create(&tcb2, NULL, reader, NULL);
	if (rv)
		puts("Failed to create thread");

	pthread_exit(NULL);
	return 0;
}
