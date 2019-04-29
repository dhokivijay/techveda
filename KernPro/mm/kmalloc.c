#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>

static unsigned char *ptr;
static unsigned char *new;

int init_module(void)
{
	pr_info("Size of struct page = %ld\n",sizeof(struct page));
	ptr = kzalloc(1024, GFP_KERNEL);				//allocate a zero-initializes buffer
	pr_info("Value of ptr (Linear Address) = %p\n",ptr);
	strcpy(ptr,"data written to page\n");

	new = krealloc(ptr, 4096, GFP_KERNEL);
	pr_info("Value of new (Linear Address) = %p\n",new);
	return 0;
}

void cleanup_module(void)
{
	kfree(new);
	pr_info("Unloading\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Veda");
