#include<stdio.h>

struct my
{
	/* create a double-word aligned int pair */
	int a[2] __attribute__ ((aligned (8)));
};


int main()
{
	struct my obj;
	double *p;
	/* a is alinged on a 16-byte boundary */
	int a __attribute__ ((aligned (16)));
	
	obj.a[0]=10;
	obj.a[1]=20;
	printf("\n %d %d \n",obj.a[0],obj.a[1]);
	
	p=(double *) &obj;
	*p = 100.9;
	printf("\n %lf \n",*p);


	
	
}
