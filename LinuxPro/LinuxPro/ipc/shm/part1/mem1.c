# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/types.h>
# include <stdlib.h>
# include <stdio.h>

int main()
{
	int shm_id;
	key_t key = 99999;
	shm_id = shmget(key,8000,IPC_CREAT);
	if(shm_id < 0){
		perror("shmget:");
		exit(1);
	}
	printf("shm_id = %d\n",shm_id);
	getchar();
	return 0;
}
