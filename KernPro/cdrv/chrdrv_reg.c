#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define CHAR_DEV_NAME "vDev3"
#define SUCCESS 0

static struct class *veda_class;
static dev_t mydev;
static int inuse = 0;
static unsigned int major = 0;

static int char_dev_open(struct inode *inode, struct file *file)
{
	if (inuse) {
		pr_err("Device busy %s", CHAR_DEV_NAME);
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

/* Register Driver with I/O abstarction layer
 *
 *  step 1: Reserve Driver/Device ID.
 *  step 2: Register file operations
 *
*/
static int __init char_dev_init(void)
{
	int ret;
	unsigned int minor = 0;

	ret = register_chrdev(major, "CHAR_DEV_NAME", &char_dev_fops);
	if (ret < 0) {
		pr_err("failed acquiring major no");
		return ret;
	}
	pr_info("major no assigned is %d", ret);
	major = ret;
	mydev = MKDEV(major, minor);

	veda_class = class_create(THIS_MODULE, "VIRTUAL");
	device_create(veda_class, NULL, mydev, NULL, "%s", "vDev3");

	return SUCCESS;
}

static void __exit char_dev_exit(void)
{
	unregister_chrdev(major, CHAR_DEV_NAME);
	device_destroy(veda_class, mydev);
	class_destroy(veda_class);
	pr_info("Driver unregistered");
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("VEDA");
MODULE_DESCRIPTION("Character Device Driver - Test");
MODULE_LICENSE("GPL");
/* End of code */
