#include <stdio.h>

void f2(void);

void f1()
{
	printf("lib call f1()\n");
	printf("new change\n");
	f2();
}

void f2()
{
	printf("lib call f1()\n");
}
