#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/delay.h>


static struct timer_list timer;
static struct task_struct *tsk = NULL;

DECLARE_WAIT_QUEUE_HEAD(queue);
DECLARE_WAIT_QUEUE_HEAD(queue2);

static volatile int wakeup;
static volatile int wakeup2;

static void periodic_executor(unsigned long ptr)
{
	wakeup = 1;
	wake_up_interruptible(&queue);
	timer.expires = jiffies + HZ;
	add_timer(&timer);
}

int kthread(void *data)
{
	int r;

	while (1) {
//		pr_info("%s: Inside kthread\n",__func__);
		r = wait_event_interruptible(queue, wakeup);
		pr_info("%s: hello, world\n",__func__);
		if (kthread_should_stop())
			break;
		wakeup = 0;
	}
	wakeup2 = 1;
	wake_up_interruptible(&queue2);
	return 0;
}

int init_module(void)
{
	pr_info("%s: Loading kthread + timer example\n",__func__);
	pr_info("%s: Will display <Hello, world> continuously...\n",__func__);
	tsk = kthread_create(kthread, NULL, "mykthread");
	if (tsk)
		wake_up_process(tsk);

	pr_info("%s: Continuing inside init_module\n",__func__);

	init_timer(&timer);
	timer.function = periodic_executor;
	timer.data = 0;
	timer.expires = jiffies + HZ;
	add_timer(&timer);

	mdelay(2000);
	pr_info("%s: Still inside init_module, has the timer fired ???\n",__func__);

	return 0;

}

void cleanup_module(void)
{
	int ret;
	pr_info("%s: Dis-arming timer\n",__func__);
	ret = kthread_stop(tsk);
	if (ret) {
		pr_info("%s: Thread stopped, return value = %d\n",__func__, ret);
	}
	ret = wait_event_interruptible(queue2, wakeup2);
	del_timer(&timer);
}

MODULE_AUTHOR("support@techveda.org");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKD_CW: Timer module");

