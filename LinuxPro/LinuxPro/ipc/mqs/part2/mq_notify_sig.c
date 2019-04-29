
/* mq_notify_sig.c

   Demonstrate message notification via signals (catching the signals with
   a signal handler) on a POSIX message queue.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>              /* For definition of O_NONBLOCK */


#define DEATH(mess) { perror(mess); exit(errno); }

#define NOTIFY_SIG SIGUSR1

#define NAME "/mymq"

static void handler(int sig)
{
	printf("\n Singal \n");
    /* Just interrupt sigsuspend() */
}

int main()
{
    struct sigevent sev;
    mqd_t mqd;
    struct mq_attr attr;
    void *buffer;
    ssize_t numRead;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;


    mqd = mq_open(NAME, O_CREAT | O_RDONLY);
    if (mqd == (mqd_t) -1) 
        DEATH("mq_open");

    /* Determine mq_msgsize for message queue, and allocate an input buffer
       of that size */

    if (mq_getattr(mqd, &attr) == -1)
        DEATH("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL) 

    /* Block the notification signal and establish a handler for it */

   sigemptyset(&blockMask);
    sigaddset(&blockMask, NOTIFY_SIG);

    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
        DEATH("sigprocmask");

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = NOTIFY_SIG;
    if (mq_notify(mqd, &sev) == -1) 
        DEATH ("mq_notify");

    sigemptyset(&emptyMask);

    for (;;) {
        sigsuspend(&emptyMask);         /* Wait for notification signal */
	
	
	while ((numRead = mq_receive(mqd, buffer, attr.mq_msgsize, NULL)) >= 0)
            printf("Read %ld bytes\n", (long) numRead);


        /* Reregister for message notification */

        if (mq_notify(mqd, &sev) == -1)
            DEATH ("mq_notify");

    }
}
