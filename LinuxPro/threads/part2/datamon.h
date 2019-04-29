#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct shdata {
	int a;
	int b;
	pthread_mutex_t lock;
};

int obj_init(struct shdata *);
int obj_write(struct shdata *, int, int);
int obj_read(struct shdata *);
