#include<linux/init.h>
#include<linux/module.h>
#include<linux/interrupt.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/timer.h>

#define SHARED_IRQ 1
static int irq= SHARED_IRQ,mydev_id,irq_counter=0;
 
static void t_func(unsigned long t_arg);

static DECLARE_TASKLET(t_name,t_func,0);

static struct simp{
	int i;
	int j;
}t_data;

//static DECLARE_TASKLET(t_name,t_func,(unsigned long)&t_data);

static int my_init(void)
{	dump_stack();
	t_data.i=100;
	t_data.j=100;
	pr_info("schedule my tasklet,jiffies =%ld \n",jiffies);
	tasklet_schedule(&t_name);
	return 0;
}

void my_exit(void)
{
	dump_stack();
	pr_info("\n hello clean up module %p\n",cleanup_module);
}

static void t_func(unsigned long t_arg)
{
	struct simp *datum;
	datum=(struct simp *)t_arg;
	pr_info("entering t_fun,datum->i = %d,jiffies=%ld\n",datum->i,jiffies);
	pr_info("entering t_fun, , in context of %s with pid= %d\n",current->comm,current->pid);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");

