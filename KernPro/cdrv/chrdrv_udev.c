#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


#define CHAR_DEV_NAME "vDev3"
#define SUCCESS 0

static struct cdev *veda_cdev;
static struct class *veda_class;
static dev_t mydev;
static int count = 1, inuse = 0;

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
	int ret, fminor = 0;

	ret = alloc_chrdev_region(&mydev, fminor, count, CHAR_DEV_NAME);
	    if (ret < 0) {
		pr_err("failed to reserve major/minor range");
		return -ret;
	}

	if (!(veda_cdev = cdev_alloc())) {
		pr_err("cdev_alloc() failed");
		unregister_chrdev_region(mydev, count);
		return -ENOMEM;
	}
	cdev_init(veda_cdev, &char_dev_fops);

	ret = cdev_add(veda_cdev, mydev, count);
	if (ret < 0) {
		pr_err("Error registering device driver");
		cdev_del(veda_cdev);
		unregister_chrdev_region(mydev, count);
		return ret;
	}

/// this two lines actually are device trees part for a real driver this 2lines get removed
	veda_class = class_create(THIS_MODULE, "VIRTUAL");
	device_create(veda_class, NULL, mydev, NULL, "%s", "vDev3");
////////////
	
	pr_info("Device Registered: %s", CHAR_DEV_NAME);
	pr_info("Major number = %d, Minor number = %d", MAJOR(mydev),
		MINOR(mydev));

	return SUCCESS;
}

static void __exit char_dev_exit(void)
{
	cdev_del(veda_cdev);
	unregister_chrdev_region(mydev, count);

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
