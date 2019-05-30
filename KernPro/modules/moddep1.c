/****************************************************
macros defined in printk.h	
     pr_emerg(fmt, ...)
     pr_alert(fmt, ...) 
     pr_crit(fmt, ...) 
     pr_err(fmt, ...) 
     pr_warning(fmt, ...) 
     pr_notice(fmt, ...) 
     pr_info(fmt, ...) 
     pr_cont(fmt, ...) 
     pr_dbg(fmt, ...)
******************************************************/
#include <linux/module.h>


static int *ptr;

int init_module (void)
{
	ptr = (int *) __symbol_get ("veda_counter");
	if (ptr)
	  {
		  *ptr = 800;
		  __symbol_put ("veda_counter");
		  return 0;
	  }
	else
	  {
		  pr_err ("Symbol not available\n");
	  }
	return 0;
}

void cleanup_module (void)
{
}

MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW : Accessing exported data symbols");
MODULE_LICENSE ("GPL");
