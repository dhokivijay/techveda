#include <linux/module.h>
#include <linux/fs.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

static struct workqueue_struct *my_workq;
static struct work_struct work;

static void w_fun(struct work_struct *w_arg)
{
	pr_info("%s: I am in w_fun, jiffies = %ld\n", __func__,jiffies);
	pr_info("%s: current task name:%s pid:%d\n", __func__,current->comm, (int)current->pid);
}

static int __init my_init(void)
{
	pr_info("%s: Executed.\n",__func__);
	my_workq = alloc_workqueue("my_workq", WQ_HIGHPRI, WQ_DFL_ACTIVE);
	INIT_WORK(&work, w_fun);
	queue_work(my_workq, &work);
        pr_info("%s: queued deferred routine, jiffies=%ld\n",__func__, jiffies);
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("%s: Executed.\n",__func__);
	destroy_workqueue(my_workq);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("LKD_CW: workqueue new api");
MODULE_LICENSE("GPL");
