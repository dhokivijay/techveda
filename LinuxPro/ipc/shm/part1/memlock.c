/* shows how to use shared memory
Version : 1.0
Author : Team -C
*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/types.h>
# include "memkey.h"

main(){
	int shm_id,ret;
	key_t key = IPC_KEY;
	shm_id = shmget(key,8000,IPC_CREAT);
	if(shm_id <0){
		perror("shmget:");
		exit(1);
	}
	printf("shm_id = %d\n",shm_id);
	ret = shmctl(shm_id, SHM_LOCK, NULL);
	if(ret <0){
	         perror("SHMCTL LOCK :");
		 exit(1);
	}
       sleep(10);
       
	ret = shmctl(shm_id, SHM_UNLOCK, NULL);
	if(ret <0){
	         perror("SHMCTL LOCK :");
		 exit(1);
	}
	
	
}
