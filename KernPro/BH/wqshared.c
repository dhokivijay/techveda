#include <linux/module.h>
#include <linux/fs.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

static void w_fun(struct work_struct *w_arg)
{
	pr_info("%s: I am in w_fun, jiffies = %ld\n", __func__,jiffies);
	pr_info("%s: current task name:%s pid:%d\n",__func__, current->comm, (int)current->pid);
}
/* step 1: create struct work_struct instance and initialize it with BH routine  */

static  DECLARE_WORK(work, (work_func_t)w_fun);

/* step 2: schedule work object by adding it to workqueue(shared). */

static int __init my_init(void)
{
	pr_info("%s: Executed.\n",__func__);
	pr_info("%s: About to schedule workqueue\n",__func__);
        schedule_work(&work);
        pr_info("%s: Queued work, jiffies=%ld\n", __func__,jiffies);
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("%s: Executed.\n",__func__);
}
module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("LKD_CW: shared workqueues");
MODULE_LICENSE("GPL");
