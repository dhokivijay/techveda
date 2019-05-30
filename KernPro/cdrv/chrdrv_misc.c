#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>

#define DEV_NAME "vDevMisc"
#define SUCCESS 0

static int inuse = 0;

static int char_dev_open(struct inode *inode, struct file *file)
{
	if (inuse) {
		pr_err("Device busy %s", DEV_NAME);
		return -EBUSY;
	}
	inuse = 1;
	pr_info("Open operation invoked");
	return SUCCESS;
}

static int char_dev_release(struct inode *inode, struct file *file)
{
	inuse = 0;
	return SUCCESS;
}

static ssize_t char_dev_write(struct file *file, const char __user * buf,
			      size_t lbuf, loff_t * offset)
{
	pr_info("Rec'vd data : %s, of len=%ld", buf, lbuf);
	return lbuf;
}

static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release
};

static struct miscdevice vDevMisc = {
	.minor = MISC_DYNAMIC_MINOR,	/* allocate misc minor */
	.name = DEV_NAME,
	.fops = &char_dev_fops,		/* drivers fops instance */
};

/* Register Driver with Misc interface */
static int __init char_dev_init(void)
{
	int ret;
	ret = misc_register(&vDevMisc);
	if (ret < 0) {
		pr_err("failed registering with minor %d", vDevMisc.minor);
		return ret;
	}
	pr_info("driver registered with Minor %d", vDevMisc.minor);

	return SUCCESS;
}

static void __exit char_dev_exit(void)
{
	misc_deregister(&vDevMisc);
	pr_info("Misc dev unregistered");
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("VEDA");
MODULE_DESCRIPTION("Character Device Driver - Test");
MODULE_LICENSE("GPL");
/* End of code */
