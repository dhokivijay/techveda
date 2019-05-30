#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <datamon.h>

int obj_init(struct shdata *obj)
{
	int ret;
	obj->a=0;
	obj->b=0;
	ret = pthread_mutex_init(&obj->lock, NULL);
        if (ret > 0)
                return -1;
        return 0;
}


int obj_write(struct shdata *obj, int newa, int newb)
{
	int ret;
	ret = pthread_mutex_lock(&obj->lock);
	obj->a = newa;
	obj->b = newb;
	ret = pthread_mutex_unlock(&obj->lock);
	return 0;
}

int obj_read(struct shdata *obj)
{
	int ret;
	ret = pthread_mutex_lock(&obj->lock);
	printf("obj.a = %d\n", obj->a);
	printf("obj.b = %d\n", obj->b);
	ret = pthread_mutex_unlock(&obj->lock);
	return 0;
}
