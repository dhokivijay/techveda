#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/version.h>

static int veda_counter;

void set_counter (int a)
{
	veda_counter = a;
}

int get_counter (void)
{
	return veda_counter;
}

static int myinit (void)
{
	pr_info ("intializing counter\n ");
	veda_counter = 100;
	return 0;
}

static void myexit (void)
{
	pr_info ("value of veda_counter %d\n ", veda_counter);
}

module_init (myinit);
module_exit (myexit);

EXPORT_SYMBOL_GPL (set_counter);
EXPORT_SYMBOL_GPL (get_counter);

MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW: exporting functions");
MODULE_LICENSE ("GPL");
