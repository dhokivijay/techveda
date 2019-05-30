
/* msg_unlink.c

   Usage: pmsg_unlink mq-name

   Unlink a POSIX message queue.

*/
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0){
        printf("%s mq-name\n", argv[0]);
	exit(1);
	}

    if (mq_unlink(argv[1]) == -1)
        perror("mq_unlink");
    exit(EXIT_SUCCESS);
}
