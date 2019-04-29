#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int glob = 0;

/* Loop 'arg' times incrementing 'glob' */
static void * threadFunc(void *arg)
{
	int local, j;
	int loops = *((int *)arg);

	for (j = 0; j < loops; j++) {
		__sync_fetch_and_add(&glob, 1); /* gnuc atomic op macro */
	}
	return NULL;
}

int main()
{
	pthread_t t1, t2;
	int loops = 10000000, ret;

	ret = pthread_create(&t1, NULL, threadFunc, &loops);
	if (ret != 0)
		perror("Pthread Create :   ");

	ret = pthread_create(&t2, NULL, threadFunc, &loops);
	if (ret != 0)
		perror("Pthread Create:    ");

	ret = pthread_join(t1, NULL);
	if (ret != 0)
		perror("Pthread Join:     ");

	ret = pthread_join(t2, NULL);
	if (ret != 0)
		perror("Pthreaf Join :    ");

	printf("glob = %d\n", glob);
	return 0;
}

/* Gnu compiler provides following atomic operation macros

Atomic fetch and operation macros
------------------------------------
        1. type __sync_fetch_and_add (type *ptr, type v, ...)
        2. type __sync_fetch_and_sub (type *ptr, type v, ...)
        3. type __sync_fetch_and_or (type *ptr, type v, ...)
        4. type __sync_fetch_and_and (type *ptr, type value, ...)
        5. type __sync_fetch_and_xor (type *ptr, type value, ...)

        All above functions return value that had previously been in memory
        before performing the suggested operation.

Atomic operation and fetch macros
------------------------------------
        1. type __sync_add_and_fetch (type *ptr, type value, ...)
        2. type __sync_sub_and_fetch (type *ptr, type value, ...)
        3. type __sync_or_and_fetch (type *ptr, type value, ...)
        4. type __sync_and_and_fetch (type *ptr, type value, ...)
        5. type __sync_xor_and_fetch (type *ptr, type value, ...)
        6. type __sync_nand_and_fetch (type *ptr, type value, ...)

        All above functions return new value in memory after performing
        the suggested operation.
*/

