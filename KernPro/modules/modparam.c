/* Notes:check this  with modinfo */

#include <linux/module.h>
#include <linux/moduleparam.h>


static int base_port[5];	/* array of integers, minimum 1, max. 5 */
static int irq;			/* single integer */

static char *name = 0;		/* string name, passed as a char pointer */


module_param_array (base_port, int, NULL, 0);
MODULE_PARM_DESC (base_port,
		  "  Int. base_port=v1,v2,... Min 1, Max 5 values");

module_param (irq, int, 0644);
MODULE_PARM_DESC (irq, "Integer");

module_param (name, charp, 0644);
MODULE_PARM_DESC (name, "String. Device Driver Name");


int init_module (void)
{
	int i;
	pr_info ("IRQ=%d, Device Name=%s", irq, name);
	for (i = 0; i < 5; ++i)
		pr_info ("Base Port %d = %x", i + 1, base_port[i]);
	return 0;
}

void cleanup_module (void)
{
        pr_info ("IRQ=%d, Device Name=%s", irq, name);
	pr_info ("Unloading");
}


MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW: module params");
MODULE_LICENSE ("GPL");
