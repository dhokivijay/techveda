#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/gfp.h>
#include <linux/mm.h>

int mod_init(void)
{
	struct page *p;
	char *lmem;
	int i;
	unsigned int pfn,size;
	
	size = PAGE_SIZE;
	pr_info("Page Size = %d\n", size);

	p = alloc_pages(GFP_KERNEL, 2);
	if (!p) {
		pr_err("Allocation failed\n");
		return -ENOMEM;
	}

	lmem = page_address(p);

	memset(lmem, 'A', PAGE_SIZE * 4);

	for (i = 0; i < (PAGE_SIZE * 4); i++)
		pr_info("%c", lmem[i]);

	__free_pages(p, 2);
	

	return 0;
}
void mod_exit(void)
{

}
module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techveda.org");


