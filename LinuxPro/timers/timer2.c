/*
   Demonstrate the use of a timer to place a timeout on a blocking system call
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define BUF_SIZE 200

static void handler(int sig)
{
    printf("Caught signal\n");          
}

int main()
{
    struct sigaction sa;
    char buf[BUF_SIZE];
    ssize_t numRead;


    /* Set up handler for SIGALRM. Allow system calls to be interrupted */

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1){
        perror("sigaction");
	}

    alarm(2);

    numRead = read(STDIN_FILENO, buf, BUF_SIZE - 1);
    /* Determine result of read() */
    if (numRead == -1) {
        if (errno == EINTR)
            printf("Read timed out\n");
        else
            perror("read");
    } else {
        printf("Successful read (%ld bytes): %.*s",
                (long) numRead, (int) numRead, buf);
    }

    exit(EXIT_SUCCESS);
}
