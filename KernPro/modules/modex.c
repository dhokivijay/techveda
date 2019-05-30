#include<linux/module.h>

int veda_counter;

int myinit (void)
{
	printk ("intializing counter");
	veda_counter = 100;
	return 0;
}

void myexit (void)
{
	printk (KERN_INFO "value of counter %d ", veda_counter);
}

module_init(myinit);
module_exit(myexit);

/****************************************
 * EXPORT_SYMBOL(sym)			*
 * EXPORT_SYMBOL_GPL(sym)		*
 * EXPORT_SYMBOL_GPL_FUTURE(sym)	*
 * EXPORT_UNUSED_SYMBOL(sym)		*
 * EXPORT_UNUSED_SYMBOL_GPL(sym)	*
****************************************/

EXPORT_SYMBOL_GPL (veda_counter);

MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW: Exporting data symbols");
MODULE_LICENSE ("GPL");
