#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>         
#include <linux/kthread.h>	
#include <linux/sched.h>	
#include <linux/semaphore.h>    
#include <linux/delay.h>	

/*
 * data package passed to threads
 */
struct kthr_data {
	const char *name;	/* kthread's name */
	struct semaphore *sem1; /* kthread waits for this semaphore */
	struct semaphore *sem2; /* kthread posts this sempahore */ 
};

static struct kthr_data prod, cons;
static struct semaphore psem, csem;
static struct task_struct *pthr, *cthr;

int prod_fct(void *data)
{
	struct kthr_data *pdata = (struct kthr_data*)data;
	while(1) {
		down(pdata->sem1);
		pr_info("%s:%s producer woken up, producing data\n", __func__, pdata->name);
		mdelay(100);
		pr_info("%s: wake up consumer\n",__func__);
		up(pdata->sem2);
		if (kthread_should_stop())
			break;
	}
	return 0;
}

int cons_fct(void *data)
{
        struct kthr_data *pdata = (struct kthr_data*)data;
        while(1) {
                down(pdata->sem1);
                pr_info("%s:wake up by producer, starting to consume....\n", __func__);
                mdelay(100);
                pr_info("%s: Done consuming, notify producer & wait for next chunck\n",__func__);
                up(pdata->sem2);
                if (kthread_should_stop())
                        break;
        }
        return 0;
}

int __init kthr_init(void)
{
	sema_init(&psem, 1); 
	sema_init(&csem, 0);

	prod.name = "producer"; 
	cons.name = "consumer";

	prod.sem1 = &psem; 
	prod.sem2 = &csem;

	cons.sem1 = &csem; 
	cons.sem2 = &psem;

	pthr = kthread_run(prod_fct, &prod,"producer");
	if(IS_ERR(pthr)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(pthr);
        }

	cthr = kthread_run(cons_fct, &cons,"consumer");
	if(IS_ERR(cthr)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(cthr);
        }

	return 0;
}

void __exit kthr_exit(void)
{
	kthread_stop(pthr);
	kthread_stop(cthr);
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("sync with kernel semaphores");
MODULE_LICENSE("GPL");

