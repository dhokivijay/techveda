#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

extern void set_counter (int);
extern int get_counter (void);

int init_module (void)
{
	int a;
	set_counter (200);
	a = get_counter ();
	pr_info ("counter set to %d \n", a);
	return 0;
}

void cleanup_module (void)
{
}

MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW : invoking exported fucntions");
MODULE_LICENSE ("GPL");
