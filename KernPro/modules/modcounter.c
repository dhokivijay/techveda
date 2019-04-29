/********************************************************************************
* Notes		: Even though the modules declares the variable 		*
*		  veda_counter as static and is not exported, 			*
                  it appears in /proc/kallsyms					*
*		  which displays its address in memory				*
\********************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

static int veda_counter;

int init_module(void)
{
	pr_info ("Initializing veda_counter to 100\n");
	veda_counter = 100;
	return 0;
}

void cleanup_module(void)
{
	pr_info ("Unloading hello\n");
	pr_info ("Value of veda_counter = %d\n",veda_counter);
}

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("LKD_CW: ");
MODULE_LICENSE("GPL");

