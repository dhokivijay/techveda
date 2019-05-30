/* Kernel Atomic macros

   The simplest of the approaches to kernel synchronization
   The easiest to understand and use. 
   Complex locking primitives are built from them.
   Atomic operations operate in one **uninterruptible* operation

   Usage Examples:
     atomic_t v;         // declaration (signed integer)

     atomic_set(&v, 5);  // assignment
     atomic_add(3, &v);  // atomic addition
     atomic_dec(&v);     // atomic decrement
     atomic_read(&v));   // return value 
     atomic_inc(&v);	 // atomic increment

   Under the hood:
   ==============
     typedef struct { 
		int counter; 
	} atomic_t;

     #define atomic_set(v, i)  ((v)->counter = (i))
     #define atomic_read(v)  ((v)->counter)
     void atomic_add(int i, atomic_t *v); // these implemented in assembly 
     void atomic_sub(int i, atomic_t *v);
     void atomic_inc(atomic_t *v);
     void atomic_dec(atomic_t *v);

*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/current.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/atomic.h>

#define LOOPS 100000

static atomic_t counter;
struct task_struct *t1, *t2;

int Kthread_start(void *arg)
{
	int i;
	for (i = 0; i < LOOPS; i++)
		atomic_inc(&counter);

	pr_info("%s: %s exit with counter at %d\n",__func__, current->comm, atomic_read(&counter));
	return 0;
}

int kthr_init(void)
{
	t1 = kthread_create(Kthread_start, NULL, "Kthread1");
	if(IS_ERR(t1)){
		pr_err("%s: unable to start kernel thread\n",__func__);
		return PTR_ERR(t1);
		
	}
	wake_up_process(t1);

	t2 = kthread_create(Kthread_start, NULL, "Kthread2");
	if(IS_ERR(t1)){
		pr_err("%s: unable to start kernel thread\n",__func__);
		return PTR_ERR(t1);
	}
	wake_up_process(t2);
	
	return 0;
}

void kthr_exit(void)
{
	pr_info("%s: counter %d\n",__func__, atomic_read(&counter));
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("Kernel Atomic macros");
MODULE_LICENSE("GPL");

