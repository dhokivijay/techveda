/*
   Send a message to a POSIX message queue.
*/
#include <mqueue.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

#define NAME "/mymq"           
int main()
{
    int flags, opt;
    mqd_t mqd;
    unsigned int prio = 60;

    flags = O_WRONLY ;
    mqd = mq_open(NAME , flags);
    if (mqd == (mqd_t) -1)
        perror("mq_open");

    if (mq_send(mqd,"writing message to queue", 25 , prio) == -1)
        perror("mq_send");

	prio = 70;
    if (mq_send(mqd,"writing message to queue", 25 , prio) == -1)
        perror("mq_send");

    exit(EXIT_SUCCESS);
}
