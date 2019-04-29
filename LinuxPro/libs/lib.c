#include <stdio.h>
x()
{
	printf("%s:routine invoked\n", __func__);
}

y()
{
	printf("%s:routine invoked\n", __func__);
	b();
}
