#include <pthread.h>
#define DATA "/mtxmydata"

struct mydata {
	pthread_mutex_t mtxlock;
	int a;
	int b;
};
