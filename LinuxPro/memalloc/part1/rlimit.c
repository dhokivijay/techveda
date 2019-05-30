#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main()
{
	int ret;
	struct rlimit slim;

	ret = getrlimit(RLIMIT_STACK, &slim);
	if(ret)
		perror("getrlimit ");
	printf(" %u MB \n",slim.rlim_cur/(1024*1024));


}
