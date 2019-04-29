#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/current.h>
#include <linux/mm.h>
#include <linux/slab.h>

int init_module(void)
{
	struct task_struct *tsk = current;
        struct mm_struct *mm = tsk->mm;
        unsigned long stack_size = (mm->stack_vm << PAGE_SHIFT);
        unsigned long down_to = mm->start_stack - stack_size;
        int len;

        pr_info( "\nInfo from the Memory Management structure " );
        pr_info(   "for task \'%s\' ", tsk->comm );
        pr_info( "(pid=%d) \n", tsk->pid );
        pr_info( "   pgd=%08lX  ", (unsigned long)mm->pgd );
        pr_info( "mmap=%08lX  ", (unsigned long)mm->mmap );
        pr_info( "map_count=%d  ", mm->map_count );
        pr_info( "mm_users=%d  ", mm->mm_users.counter );
        pr_info( "mm_count=%d  ", mm->mm_count.counter );
        pr_info( "\n" );
        pr_info( "    start_code=%08lX  ", mm->start_code );
        pr_info( " end_code=%08lX\n", mm->end_code );
        pr_info( "    start_data=%08lX  ", mm->start_data );
        pr_info( " end_data=%08lX\n", mm->end_data );
        pr_info( "     start_brk=%08lX  ", mm->start_brk );
        pr_info( "      brk=%08lX\n", mm->brk );
        pr_info( "     arg_start=%08lX  ", mm->arg_start );
        pr_info( "  arg_end=%08lX\n", mm->arg_end );
        pr_info( "     env_start=%08lX  ", mm->env_start );
        pr_info( "  env_end=%08lX\n", mm->env_end );
        pr_info( "   start_stack=%08lX  ", mm->start_stack );
        pr_info( "  down_to=%08lX ", down_to );
        pr_info( " <--- stack grows downward \n" );

	return 0;
}

void cleanup_module(void)
{
	pr_info("Unloading\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Veda");
