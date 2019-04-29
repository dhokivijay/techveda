#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

void *ptr;
int init_module(void)
{
	ptr = vmalloc(6000);
	pr_info("Value of ptr (Virtual Address) = %p\n",ptr);
	strcpy(ptr,"data written to page\n");
	return 0;
}

void cleanup_module(void)
{
	vfree(ptr);
	pr_info("Unloading\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Veda");
