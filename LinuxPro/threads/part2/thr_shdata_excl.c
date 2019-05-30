#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int a;
	int b;
}DATA;


DATA glob; /* shared data object */

int ex_flag;  /* exclusion counter */

	
static void * writer(void *arg) /* Thread 1 */
{

LOCK:
    if(__sync_bool_compare_and_swap(&ex_flag, 1, 0)) // lock operation
    {
    	glob.a=10;
    	sleep(1);
    	glob.b=20;
        __sync_bool_compare_and_swap(&ex_flag, 0, 1); // unlock operation
    }
    else
	goto LOCK; // re-attempt
	
    return NULL;
}

static void * reader(void *arg)		/* Thread 2 */
{

LOCK1:    

    if(__sync_bool_compare_and_swap(&ex_flag, 1, 0))  
    {
      printf("glob.a = %d\n",glob.a);
      printf("glob.b = %d\n",glob.b);
      __sync_bool_compare_and_swap(&ex_flag, 0, 1);  
    }
    else
	goto LOCK1; // re-attempt 

    return NULL;
}

int main()
{

    pthread_t t1, t2;
    int ret;

    /* init lock counter */
    __sync_add_and_fetch(&ex_flag, 1);

   /* init threads */
    ret = pthread_create(&t1, NULL, writer, NULL);
    if (ret != 0)
        perror("Pthread Create :   ");

    ret = pthread_create(&t2, NULL, reader, NULL);
    if (ret != 0)
	perror("Pthread Create:    ");

	
    pthread_exit(NULL);

}
