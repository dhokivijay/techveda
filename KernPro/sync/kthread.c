#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>         
#include <linux/kthread.h>	
#include <linux/sched.h>	
#include <linux/delay.h>	

struct task_struct *ts;

int Kthread_start(void *data)
{
	while(1) {
		pr_info("%s: Thread start routine\n",__func__);
		/* Introduce delay to simulate some work */
		msleep(500);
		if (kthread_should_stop())
			break;
	}
	return 0;
}

int __init kthr_init(void)
{
	ts = kthread_create(Kthread_start, NULL, "Mykthread");
	if(IS_ERR(ts)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(ts);
        }
	/* start kthread execution */
        wake_up_process(ts);
	
	pr_info("%s:Kthread created with id %d\n", __func__, ts->pid);
	return 0;
}

void __exit kthr_exit(void)
{
	kthread_stop(ts);
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("Simple Kthread");
MODULE_LICENSE("GPL");

