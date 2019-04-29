/* msg_file_server.c

   A file server that uses System V message queues to handle client requests
   (see msg_file_client.c). The client sends an initial request containing
   the name of the desired file, and the identifier of the message queue to be
   used to send the file contents back to the child. The server attempts to
   open the desired file. If the file cannot be opened, a failure response is
   sent to the client, otherwise the contents of the requested file are sent
   in a series of messages.

   This application makes use of multiple message queues. The server maintains
   a queue (with a well-known key) dedicated to incoming client requests. Each
   client creates its own private queue, which is used to pass response
   messages from the server back to the client.

   This program operates as a concurrent server, forking a new child process to
   handle each client request while the parent waits for further client requests.
*/
#include "msgfile.h"

static void childReaper(int sig)         /* SIGCHLD handler */
{
    int savedErrno;

    savedErrno = errno;                 /* waitpid() might change 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}

static void serveRequest(const struct requestMsg *req)         /* process each request */
{
    int fd;
    ssize_t numRead;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {                     /* Open failed: send error text */
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);
        exit(EXIT_FAILURE);             /* and terminate */
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. We don't
       diagnose read() and msgsnd() errors since we can't notify client. */

    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
        if (msgsnd(req->clientId, &resp, numRead, 0) == -1)
            break;

    /* Send a message of type RESP_MT_END to signify end-of-file */

    resp.mtype = RESP_MT_END;
    msgsnd(req->clientId, &resp, 0, 0);         /* Zero-length mtext */
}

int main(int argc, char *argv[])
{
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa;

    /* Create server message queue */

    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL |
                            S_IRUSR | S_IWUSR | S_IWGRP);
    if (serverId == -1)
        perror("msgget");

    /* Establish SIGCHLD handler to reap terminated children */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = childReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        perror("sigaction");

    /* Read requests, handle each in a separate child process */

    for (;;) {
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            if (errno == EINTR)         /* Interrupted by SIGCHLD handler? */
                continue;               /* ... then restart msgrcv() */
            perror("msgrcv");           /* Some other error */
            break;                      /* ... so terminate loop */
        }

        pid = fork();                   /* Create child process */
        if (pid == -1) {
            perror("fork");
            break;
        }

        if (pid == 0) {                 /* Child handles request */
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }

        /* Parent loops to receive next client request */
    }

    /* If msgrcv() or fork() fails, remove server MQ and exit */

    if (msgctl(serverId, IPC_RMID, NULL) == -1)
        perror("msgctl");
    exit(EXIT_SUCCESS);
}
