/* 
   Receive a message from a POSIX message queue, and write it on
   standard output.

   Linux supports POSIX message queues since kernel 2.6.6.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>              

#define NAME "/mymq"


int main()
{
    int flags, opt;
    mqd_t mqd;
    unsigned int prio;
    void *buffer;
    struct mq_attr attr;
    ssize_t numRead;

    flags = O_RDONLY;
    prio = 60;

    mqd = mq_open(NAME , flags);
    if (mqd == (mqd_t) -1)
        perror("mq_open");

    /* We need to know the 'mq_msgsize' attribute of the queue in
       order to determine the size of the buffer for mq_receive() */

    if (mq_getattr(mqd, &attr) == -1)
        perror("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        perror("malloc");

    numRead = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
    if (numRead == -1)
        perror("mq_receive");

    printf("Read %ld bytes; priority = %u\n", (long) numRead, prio);
    if (write(STDOUT_FILENO, buffer, numRead) == -1)
        perror("write");
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}
