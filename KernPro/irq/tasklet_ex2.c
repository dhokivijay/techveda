#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>

#define SHARED_IRQ 1
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param(irq, int, S_IRUGO);

static void t_fun(unsigned long t_arg);

static DECLARE_TASKLET(t_name, t_fun, 0);

static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	irq_counter++;
	pr_info("In the ISR: counter = %d\n", irq_counter);
	tasklet_schedule(&t_name);
	return IRQ_NONE;	/* we return IRQ_NONE because we are just observing */
}

static void t_fun(unsigned long t_arg)
{
	pr_info
	    ("Entering t_fun, jiffies = %ld , called by %x  in context of %s\n",
	     jiffies, current->pid, current->comm);
}

static int __init my_init(void)
{
	if (request_irq
	    (irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
		return -1;
	pr_info("Successfully loading ISR handler\n");
	dump_stack();
	return 0;
}

static void __exit my_exit(void)
{
	synchronize_irq(irq);
	free_irq(irq, &my_dev_id);
	pr_info("Successfully unloading \n");
	dump_stack();
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team veda");
MODULE_DESCRIPTION("Interrupt Handler Test");
MODULE_LICENSE("GPL");
