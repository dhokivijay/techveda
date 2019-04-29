#include <linux/module.h>	//kmod interface

void func (void)
{
	printk("Hello world\n");
}

int init_module (void)
{
	printk("module inserted \n");
	return 0;

}

void cleanup_module (void)
{
	printk("module removed\n");
}

/*Kernel module Comments*/
MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW: First kernel module");
MODULE_LICENSE ("GPL");
