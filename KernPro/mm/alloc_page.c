#include <linux/module.h>
#include <linux/init.h>
//#include <linux/sched.h>
#include <linux/gfp.h>
#include <linux/mm.h>

int mod_init(void)
{
	struct page *p;
	char *lmem;
	int i;
	unsigned int pfn;
	/*
	 * Allocate memory using alloc_page
	 */
	p = alloc_page(GFP_KERNEL);
	if (!p) {
		pr_err("\nCannot Allocate Memory\n");
		return -ENOMEM;
	}

	/*
	 * print page frame number corresponding to
 	 * page allocated above
	 */
	pr_info("\nPage Frame Number %ld", page_to_pfn(p));
	
	/*
	 * Get linear address for the above page
	 */
	lmem = page_address(p);
	pr_info("\nLinear Address %p\n", lmem);

	/* Write some thing into the memory, and print it*/
	memset(lmem, 'A', PAGE_SIZE);

	for (i = 0; i < PAGE_SIZE; i++)
		pr_info("%c", lmem[i]);

	/*
	 * free the page
	 */
	__free_page(p);
	
	/*Get memory using __get_free_page() */

	lmem = (char *)__get_free_page(GFP_KERNEL);
	if (!lmem) {
		pr_info("\nCannot Allocate memory\n");
		return -ENOMEM;
	}	
	pr_info("\nLinear Address %p", lmem);

	/*
	 * Get liner address page frame number
	 */
	pfn = __pa(lmem) >> PAGE_SHIFT;
	pr_info("\nPage frame number %d", pfn);

	/*
	 * Get address to page object
	 */
	pr_info("\nPage Object %p\n", pfn_to_page(pfn));
	/*Free the memory*/
	free_page((unsigned long)lmem);

	return 0;
}
void mod_exit(void)
{

}
module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("support@techveda.org");

