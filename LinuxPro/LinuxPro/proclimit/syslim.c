/*
   sysconf() to retrieve system limits.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void sysconfPrint(const char *msg,  int name)            /* Print 'msg' plus sysconf() value for 'name' */
{
    long lim;

    errno = 0;
    lim = sysconf(name);
    if (lim != -1) {        /* Call succeeded, limit determinate */
        printf("%s %ld\n", msg, lim);
    } 
   else
        printf("%s (indeterminate)\n", msg);
}

int main()
{
    sysconfPrint("_SC_ARG_MAX:        ", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX:       ", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX:    ", _SC_NGROUPS_MAX); 
    sysconfPrint("_SC_CHILD_MAX:      ", _SC_CHILD_MAX); 
    sysconfPrint("_SC_PAGESIZE:       ", _SC_PAGESIZE);
    sysconfPrint("_SC_RTSIG_MAX:      ", _SC_RTSIG_MAX);
    exit(EXIT_SUCCESS);
}
