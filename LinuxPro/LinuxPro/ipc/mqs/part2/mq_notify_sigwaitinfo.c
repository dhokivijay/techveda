/* 
   Demonstrate message notification via signals (accepting the signals with
   sigwaitinfo()) on a POSIX message queue.
*/
#define _POSIX_C_SOURCE 199309

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>              /* For definition of O_NONBLOCK */

#define NOTIFY_SIG SIGRTMIN     /* Signal used for message notifications */
#define NAME "/mymq"

int main(int argc, char *argv[])
{
    struct sigevent sev;
    mqd_t mqd;
    struct mq_attr attr;
    void *buffer;
    ssize_t numRead;
    sigset_t blockMask;
    siginfo_t si;


    mqd = mq_open(NAME, O_RDONLY | O_NONBLOCK);
    if (mqd == (mqd_t) -1){
        perror("mq_open");
	exit(1);
       }

    /* Determine mq_msgsize for message queue, and allocate an input buffer
       of that size */

    if (mq_getattr(mqd, &attr) == -1){
        perror("mq_getattr");
	exit(1);
       }
    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL) {
        perror("malloc");
 	exit(1);
	}

    /* Block the signal that we'll accept using sigwaitinfo() */

    sigemptyset(&blockMask);
    sigaddset(&blockMask, 34);
    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1){
        perror("sigprocmask");
	exit(1);
       }

    /* Set up message notification using the signal NOTIFY_SIG */

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = 34;
    sev.sigev_value.sival_ptr = &mqd;
                /* This allows us to obtain a pointer to 'mqd' in the
                   siginfo_t structure returned by sigwaitinfo() */

    if (mq_notify(mqd, &sev) == -1){
        perror("mq_notify");
	exit(1);
	}

    for (;;) {

        /* Wait for a signal; when it is received, display associated
           information */

	printf(" Invoking sigwaitinfo \n");
        if (sigwaitinfo(&blockMask, &si) == -1)
	    perror("sigwaitinfo");

        printf("Accepted signal:\n");
        printf("        si_signo   = %d\n", si.si_signo);
        printf("        si_pid     = %ld\n", (long) si.si_pid);
        printf("        si_uid     = %ld\n", (long) si.si_uid);
        printf("        *sival_ptr = %p\n\n", si.si_value.sival_ptr);

        /* Although only one signal might have been queued (if NOTIFY_SIG
           is a standard signal) we might have received multiple messages,
           so use nonblocking mq_receive() calls inside a loop to read
           as many messages as possible. */

        while ((numRead = mq_receive(mqd, buffer, attr.mq_msgsize, NULL)) >= 0)
            printf("Read %ld bytes\n", (long) numRead);

	/* Reestablish message notification */

        if (mq_notify(mqd, &sev) == -1)
            perror("mq_notify");


       }
}
