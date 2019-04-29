#include<linux/module.h>

extern int veda_counter;

int myinit (void)
{
	/* set counter  */
	veda_counter = 500;
	return 0;
}

void myexit (void)
{

}

module_init (myinit);
module_exit (myexit);

MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW : Accessing exported symbol ");
MODULE_LICENSE ("GPL");
