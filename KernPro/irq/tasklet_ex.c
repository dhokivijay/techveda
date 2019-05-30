#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/init.h>

static void t_fun(unsigned long t_arg);

static struct simp {
	int i;
	int j;
} t_data;

static DECLARE_TASKLET(t_name, t_fun, (unsigned long)&t_data);

static int __init my_init(void)
{
	t_data.i = 100;
	t_data.j = 200;
	pr_info(" scheduling my tasklet, jiffies= %ld \n", jiffies);
	tasklet_schedule(&t_name);
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("\nHello: cleanup_module loaded at address 0x%p\n",
		cleanup_module);
}

static void t_fun(unsigned long t_arg)
{
	struct simp *datum;
	datum = (struct simp *)t_arg;
	pr_info("Entering t_fun, datum->i = %d, jiffies = %ld\n",
		datum->i, jiffies);
	pr_info("Entering t_fun, ,in context of %s with pid = %d\n",
		current->comm, current->pid);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("support@techveda.org");
MODULE_LICENSE("GPL");
