#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/kernel.h>

#define SHARED_IRQ 1
static int irq = SHARED_IRQ, my_dev_id;
module_param(irq, int, S_IRUGO);

static int delay = 0;
module_param(delay, int, S_IRUGO);

static atomic_t counter_bh, counter_th;

static void t_fun(unsigned long t_arg)
{
	atomic_inc(&counter_bh);
	printk(KERN_INFO
	       "In BH: counter_th = %d, counter_bh = %d, jiffies=%ld\n",
	       atomic_read(&counter_th), atomic_read(&counter_bh), jiffies);
}

static DECLARE_TASKLET(t_name, t_fun, 0);

static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	atomic_inc(&counter_th);
	printk(KERN_INFO " current jiffies = %ld\n", jiffies);
	tasklet_schedule(&t_name);
	mdelay(delay);		/* hoke up delay to create backlogs */
	return IRQ_NONE;	/* we return IRQ_NONE because we are just observing */
}

static int __init my_init(void)
{
	if (request_irq
	    (irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
		return -1;
	printk(KERN_INFO "Successfully loading ISR handler\n");
	return 0;
}

static void __exit my_exit(void)
{
	synchronize_irq(irq);
	free_irq(irq, &my_dev_id);
	printk(KERN_INFO " counter_th = %d,  counter_bh = %d\n",
	       atomic_read(&counter_th), atomic_read(&counter_bh));

	printk(KERN_INFO "Successfully unloading \n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team veda");
MODULE_DESCRIPTION("Tasklet count Test");
MODULE_LICENSE("GPL");
