#include<linux/module.h>
#include<linux/init.h>
#include<linux/gfp.h>
#include<linux/mm.h>

int mod_init(void)
{

	struct page *p;
	char *lmem;
	int i;
	unsigned int pfn;
	p=alloc_page(GFP_KERNEL);
	if(!p)
	{
		pr_err("cannot allocate memory\n");
		return -ENOMEM;
	}
	pr_info("page frame number %ld\n",page_to_pfn(p));

	lmem=page_address(p);
	pr_info("linear address %p\n",lmem);
	memset(lmem,'A',PAGE_SIZE);
	for(i=0;i<PAGE_SIZE;i++){
		pr_info("%c\t",lmem[i]);
	}
	pr_err("\n the value of page size %ld\n",PAGE_SIZE);
	__free_page(p);

	lmem=(char *)__get_free_page(GFP_KERNEL);

	pr_info("linear address %p\n",lmem);
	pfn=__pa(lmem)>>PAGE_SHIFT;


	pr_info("page object %p\n",pfn_to_page(pfn));
	free_page((unsigned long)lmem);
	return 0;


}

void mod_exit(void){
}
module_init(mod_init);
module_exit(mod_exit);


MODULE_AUTHOR("VIJAY");	
MODULE_LICENSE("GPL");


