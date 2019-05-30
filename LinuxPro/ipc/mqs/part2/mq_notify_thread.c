/* mq_notify_thread.c
   Demonstrate message notification via threads on a POSIX message queue.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>              /* For definition of O_NONBLOCK */


#define NAME "/mymq"

static void notifySetup(mqd_t *mqdp);

static void threadFunc(union sigval sv)                    /* Thread notification function */ 
{
    ssize_t numRead;
    mqd_t *mqdp;
    void *buffer;
    struct mq_attr attr;

    mqdp = sv.sival_ptr;

    /* Determine mq_msgsize for message queue, and allocate an input buffer
       of that size */

    if (mq_getattr(*mqdp, &attr) == -1)
        perror("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        perror("malloc");


    while ((numRead = mq_receive(*mqdp, buffer, attr.mq_msgsize, NULL)) >= 0)
        printf("Read %ld bytes\n", (long) numRead);

      /* Reregister for message notification */

    notifySetup(mqdp);

    free(buffer);
    pthread_exit(NULL);
}

static void notifySetup(mqd_t *mqdp)
{
    struct sigevent sev;

    sev.sigev_notify = SIGEV_THREAD;            /* Notify via thread */
    sev.sigev_notify_function = threadFunc;
    sev.sigev_notify_attributes = NULL;
            /* Could be pointer to pthread_attr_t structure */
    sev.sigev_value.sival_ptr = mqdp;           /* Argument to threadFunc() */

    if (mq_notify(*mqdp, &sev) == -1) {
        perror("mq_notify");
	exit(1);
	}
}

int main(int argc, char *argv[])
{
    mqd_t mqd;

    mqd = mq_open(NAME , O_CREAT |  O_RDONLY | O_NONBLOCK);
    if (mqd == (mqd_t) -1){
        perror("mq_open");
	exit(1);
	}

    notifySetup(&mqd);
    pause();                    /* Wait for notifications via thread function */
}

