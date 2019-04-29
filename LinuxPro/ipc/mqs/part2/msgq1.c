#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NAME "/mymq"
#define SUCCESS 0

int main()
{
    mqd_t mqd;
    struct mq_attr attr;

    attr.mq_maxmsg = 50;
    attr.mq_msgsize = 2048;

    mqd = mq_open(NAME,O_RDWR | O_CREAT, 666, &attr);
    if (mqd == (mqd_t) -1)
        perror("mq_open");
	
    return SUCCESS;
}
