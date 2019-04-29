#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/wait.h>

#define SHARED_IRQ 1
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param(irq, int, S_IRUGO);

static DECLARE_WAIT_QUEUE_HEAD(wq);
static atomic_t cond;
static struct task_struct *tsk;

static int t_fun(void *t_arg);
static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	irq_counter++;
	pr_info("In the ISR: counter = %d\n", irq_counter);

	/* set atomic counter and wake up thread (BH) */
	atomic_set(&cond, 1);
	wake_up_interruptible(&wq);
	return IRQ_NONE;	/* we return IRQ_NONE because we are just observing */
}

static int t_fun(void *t_arg)
{
	do {
		atomic_set(&cond, 0);
		/* wait until either thread is suspended or an atomic 
		   counter is set */
		wait_event_interruptible(wq, kthread_should_stop()
					 || atomic_read(&cond));
		pr_info("Entering t_fun, pid = %d ,context of %s\n",
			current->pid, current->comm);

	} while (!kthread_should_stop());
	return 0;
}

static int __init my_init(void)
{
	if (request_irq
	    (irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
		return -1;
	/* arg1: thread routine
	 * arg2: Arg to thread routine
	 * arg3: Name to indentify thread
	 */
	tsk = kthread_run(t_fun, NULL, "mykthread");

	return 0;
}

static void __exit my_exit(void)
{
	synchronize_irq(irq);
	free_irq(irq, &my_dev_id);
	kthread_stop(tsk);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team veda");
MODULE_DESCRIPTION("Interrupt Handler Test");
MODULE_LICENSE("GPL");
