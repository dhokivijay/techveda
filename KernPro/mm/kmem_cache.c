#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/gfp.h>

struct kmem_cache *mycache;
void *object;

int kmem_init(void)
{

       mycache = kmem_cache_create("test_cache", 128, 0, SLAB_RED_ZONE, NULL);
       if (!mycache)
	    return -ENOMEM;
	
	pr_info("cache size %d\n", kmem_cache_size(mycache));
	object = kmem_cache_alloc(mycache, GFP_KERNEL);
	if (!object)
		return -ENOMEM;

	return 0;
}

void kmem_exit(void)
{
	kmem_cache_free(mycache, object);
	kmem_cache_destroy(mycache);
}

module_init(kmem_init);
module_exit(kmem_exit);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("LKD_CW: cache");
MODULE_LICENSE("GPL");
