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
static atomic_t nevents;	/* number of events to deal with */
static atomic_t catchup;	/* number of 'missed events' */

static void t_fun(unsigned long t_arg)
{
	if (atomic_read(&nevents) <= 0)
		return;

	for (;;) {
		atomic_inc(&counter_bh);
		printk
		    (KERN_INFO
		     "In BH: counter_th = %d, counter_bh = %d, jiffies= %ld\n",
		     atomic_read(&counter_th), atomic_read(&counter_bh),
		     jiffies);
		if (atomic_dec_and_test(&nevents))
			break;
		atomic_inc(&catchup);
		printk(KERN_INFO "****** nevents > 0, catchup=%d\n",
		       atomic_read(&catchup));
	}

}

static DECLARE_TASKLET(t_name, t_fun, 0);

static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	atomic_inc(&counter_th);
	atomic_inc(&nevents);
	printk(KERN_INFO " current jiffies = %ld\n", jiffies);
	tasklet_schedule(&t_name);
	mdelay(delay);		/* hoke up delay to create backlogs */
	return IRQ_NONE;	/* we return IRQ_NONE because we are just observing */
}

static int __init my_init(void)
{
	atomic_set(&catchup, 0);
	atomic_set(&nevents, 0);

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

	printk(KERN_INFO "Final statistics:   catchup = %d\n",
	       atomic_read(&catchup));
	printk(KERN_INFO "Successfully unloading \n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team veda");
MODULE_DESCRIPTION("Tasklet count Test");
MODULE_LICENSE("GPL");
