#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/gfp.h>
#include<linux/types.h>
#include <asm/atomic.h>

#define SUCCESS 0

struct kmem_cache *my_cache;

typedef struct {
	int a;
	int b;
	int c;
} private_data;

private_data *handle;

static void cache_init(void *data)
{
	private_data *mydata = (private_data *) data;
	mydata->a = 0;
	mydata->b = 0;
	mydata->c = 0;
}

static private_data *myalloc(void)
{
	private_data *mydata;
	mydata = (private_data *) kmem_cache_alloc(my_cache, GFP_KERNEL);
	return mydata;
}

static void myfree(private_data * free)
{
	kmem_cache_free(my_cache, free);
	pr_info("myfree invoked");
}

static int __init myinit(void)
{

	/* create  private slab cache list and align it with hardware cache */
	my_cache =
	    kmem_cache_create("mycache", sizeof(private_data), 0,
			      SLAB_HWCACHE_ALIGN, cache_init);
	if (my_cache == NULL)
		return -ENOMEM;

	/* Alloc an object from list */
	handle = myalloc();
	pr_info(" private data state object.a=%d object.b=%d object.c=%d",
		handle->a, handle->b, handle->c);

	/* release the object */
	myfree(handle);

	return SUCCESS;
}

static void __exit myexit(void)
{
	/* delete cache list */
	kmem_cache_destroy(my_cache);
}

module_init(myinit);
module_exit(myexit);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("LKD_CW: cache allocations");
MODULE_LICENSE("GPL");
