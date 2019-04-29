#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/mm.h>

#define DEV_NAME 	"vramdev"
#define SUCCESS 	0
#define FB_BASE 	0x000A0000
#define FB_SIZE 	0x00020000

static struct cdev *veda_cdev;
static dev_t mydev;
int count = 1;
static struct class *veda_class;
static int char_dev_open(struct inode *inode, struct file *file)
{
	return SUCCESS;
}

static int char_dev_release(struct inode *inode, struct file *file)
{
	return SUCCESS;
}
/* mmap() --> sys_mmap() --> fops_mmap() */
static int char_dev_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long region_origin = vma->vm_pgoff * PAGE_SIZE;
	unsigned long region_length = vma->vm_end - vma->vm_start;
	unsigned long physical_addr = FB_BASE + region_origin;
	unsigned long user_virtaddr = vma->vm_start;

	/* sanity check: mapped region cannot expend past end of vram */
	if (region_origin + region_length > FB_SIZE)
		return -EINVAL;
	/* set up the page-table entries */
	if (remap_pfn_range(vma, user_virtaddr, physical_addr >> PAGE_SHIFT,
			    region_length, vma->vm_page_prot))
		return -EAGAIN;
	return SUCCESS;
}

static struct file_operations fbops = {
	.owner = THIS_MODULE,
	.open = char_dev_open,
	.release = char_dev_release,
	.mmap = char_dev_mmap
};

static __init int char_dev_init(void)
{
	int ret;
	if (alloc_chrdev_region(&mydev, 0, count, DEV_NAME) < 0) {
		pr_err("failed to reserve major/minor range\n");
		return -1;
	}
	veda_cdev = cdev_alloc();
	if (veda_cdev == NULL) {
		pr_err("cdev_alloc() failed\n");
		unregister_chrdev_region(mydev, count);
		return -1;
	}
	cdev_init(veda_cdev, &fbops);
	ret = cdev_add(veda_cdev, mydev, count);
	if (ret < 0) {
		pr_err("Error registering device driver\n");
		cdev_del(veda_cdev);
		unregister_chrdev_region(mydev, count);
		return -1;
	}
	veda_class = class_create(THIS_MODULE, "VIRTUAL");
	device_create(veda_class, NULL, mydev, NULL, "%s", "vramdev");
	return SUCCESS;
}

static __exit void char_dev_exit(void)
{
	device_destroy(veda_class, mydev);
	class_destroy(veda_class);
	cdev_del(veda_cdev);
	unregister_chrdev_region(mydev, 1);
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("VEDA");
MODULE_LICENSE("GPL");
