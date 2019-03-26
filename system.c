#include<stdio.h>
#include<signal.h>

void func(int i)
{
printf("the signal %d\n",i);
}

void main()
{
int a=10;
printf("the value is %d\n",a);
while(1)
signal(SIGINT,func);
a=sysconf();
}
