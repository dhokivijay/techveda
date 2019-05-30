#include<linux/module.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/interrupt.h>
static int irq_counter,my_dev_id=1,irq=2;
static irqreturn_t my_interrupt(int irq,void *dev_id)
{
irq_counter++;
pr_info("inthe ISR counter=%d\n",irq_counter);
return IRQ_NONE;

}

static int my_init(void)
{
 
pr_info(KERN_ERR"the my interrupt init %s\n",__func__);
if(request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
	return -1;
return 0;
}


void my_exit(void)
{
synchronize_irq(irq);
free_irq(irq,&my_dev_id);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("VIJAY");
MODULE_LICENSE("GPL");
