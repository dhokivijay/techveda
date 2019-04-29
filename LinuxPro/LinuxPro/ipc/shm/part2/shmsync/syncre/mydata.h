#include <semaphore.h>

/* use posix un-named semaphore(no file interface) */
struct mydata{
	sem_t mysem;
	int a;
	int b;
}; 
