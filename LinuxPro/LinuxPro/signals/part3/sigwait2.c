#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

void timestamp( char *str ) {
    time_t t;
    time(&t);
    printf( "The time %s is %s\n", str, ctime(&t) );
}

int main( int argc, char *argv[] ) {

    int result = 0;

    sigset_t waitset;
    siginfo_t info;
    struct timespec timeout;

    sigemptyset( &waitset );
    sigaddset( &waitset, SIGALRM );
    sigprocmask( SIG_BLOCK, &waitset, NULL );

    timeout.tv_sec = 5;     /* Number of seconds to wait */
    timeout.tv_nsec =10;  /* Number of nanoseconds to wait */


    alarm(5);

    timestamp( "before sigtimedwait()" );

    result = sigtimedwait( &waitset, &info, &timeout );
    if( result <  0 ) {
	printf("sigtimedwait failed : \n");
        exit(1);
    }

    printf( "sigtimedwait returned for signal %d\n",info.si_signo );

    timestamp( "after sigtimedwait()" );

    return( result );
}
