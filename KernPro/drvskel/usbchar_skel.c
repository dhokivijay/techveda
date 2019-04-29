#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/slab.h>

#define VENDOR 0x0781
#define DEVICE 0x5567  

#define MAJORNO 190
#define MINORNO  0
#define CHAR_DEV_NAME "vDev1"
#define SUCCESS 0

static struct cdev *veda_cdev;
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
        pr_info("Rec'vd data : %s, of len=%ld\n", buf, lbuf);
        return lbuf;
}

static struct file_operations char_dev_fops = {
        .owner = THIS_MODULE,
        .write = char_dev_write,
        .open = char_dev_open,
        .release = char_dev_release
};
/* end of driver code */

                          
static int
test_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	mydev = MKDEV(MAJORNO, MINORNO);
        register_chrdev_region(mydev, count, CHAR_DEV_NAME);

        /* Allocate cdev instance */
        veda_cdev = cdev_alloc();

        /* initialize cdev with fops object */
        cdev_init(veda_cdev, &char_dev_fops);

        /* register cdev with vfs(devtmpfs) */
        ret = cdev_add(veda_cdev, mydev, count);
        if (ret < 0) {
                pr_err("Error registering device driver");
                return ret;
        }

	return 0;
}

static void test_disconnect(struct usb_interface *intf)
{

	pr_info("my_usb_disconnect\n");
}

static struct usb_device_id dev_table[] = {
	{USB_DEVICE(VENDOR,DEVICE)},	/* device */
	{}				/* Null terminator (required) */
};

MODULE_DEVICE_TABLE(usb, dev_table);

static struct usb_driver my_usb_driver = {
	.name = "usb-test",
	.probe = test_probe,  // driver's init call
	.disconnect = test_disconnect, // driver's exit call
	.id_table = dev_table,// table of devices we are looking for 
};

static int __init test_init(void)
{
	int err;
	pr_info("Hello USB\n");
	err = usb_register(&my_usb_driver);
	return err;
}

static void test_exit(void)
{
	pr_info("Goodbye USB\n");
	usb_deregister(&my_usb_driver);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team veda");
MODULE_DESCRIPTION("Usb driver initialization");
