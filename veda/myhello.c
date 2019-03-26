#include<linux/module.h>
#include<linux/init.h>
#include<linux/mm.h>
int init_module(void)
{
	pr_emerg("vijay pr \n");
	dump_stack();
	//printk("hello\n");
	return 0;

}
void cleanup_module(void)
{

	printk("hello exit function\n");
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIJAY");
