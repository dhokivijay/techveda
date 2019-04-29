#include <stdio.h>

void test()
{
	printf("%s: library test routine invoked\n",__func__);
}

void _init()
{
	printf("%s:in library init routine\n",__func__);
}

void _fini()
{
	printf("%s: in library fini routine\n",__func__);
}
