#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 1
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0, i = 0;
module_param(irq, int, S_IRUGO);

static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	irq_counter++;
	pr_info("In the ISR: counter = %d\n", irq_counter);
	return IRQ_NONE;	/* we return IRQ_NONE because we are just observing */
	/*return IRQ_HANDLED; */
}

static int __init my_init(void)
{
	for (i = 0; i < 24; i++) {
		request_irq(i, my_interrupt, IRQF_SHARED, "my_interrupt",
			    &my_dev_id);
	}
	/* arg1: irq no
	   arg2: driver's interrupt handler address
	   arg3: priority flag
	   arg4: name of the driver
	   arg5: unique no to identify interrupt handler 
	 */
	pr_info("Registered IRQ handler\n");
	return 0;
}

static void __exit my_exit(void)
{
	for (i = 0; i < 24; i++) {
		synchronize_irq(i);
		free_irq(i, &my_dev_id);
		pr_info("Successfully unloading \n");
	}
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team veda");
MODULE_DESCRIPTION("Interrupt Handler Test");
MODULE_LICENSE("GPL");
