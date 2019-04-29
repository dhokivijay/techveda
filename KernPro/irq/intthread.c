#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 1
static int irq = SHARED_IRQ, my_dev_id = 1, irq_counter = 0;
module_param(irq, int, S_IRUGO);

static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	/* ensure interupt is valid and handover control to thread handler */
	return IRQ_WAKE_THREAD;
}

static irqreturn_t my_intthread(int irq, void *dev_id)
{
	irq_counter++;
	pr_info("In the ISR: counter = %d\n", irq_counter);
	return IRQ_NONE;	/* we return IRQ_NONE because we are just observing */
	/*return IRQ_HANDLED; */
}

static int __init my_init(void)
{
	if (request_threaded_irq
	    (irq, my_interrupt, my_intthread, IRQF_SHARED,
	     "my_interrupt", &my_dev_id))
		return -1;
	pr_info("Registered IRQ handler\n");
	return 0;
}

static void __exit my_exit(void)
{
	/* verify if isr is running */
	synchronize_irq(irq);
	free_irq(irq, &my_dev_id);
	pr_info("Successfully unloading \n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team veda");
MODULE_DESCRIPTION("Interrupt Handler Test");
MODULE_LICENSE("GPL");
