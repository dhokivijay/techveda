#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/blkpg.h>
#include <linux/version.h>

static int major_num = 0;
module_param(major_num, int, 0);
static int hardsect_size = 512;
module_param(hardsect_size, int, 0);
static int nsectors = 1024;	/* How big the drive is */
module_param(nsectors, int, 0);

static unsigned long size;	//size of the device
spinlock_t lock;
static u8 *data;		//array to store disk data
static struct gendisk *gd;	//kernel representation of device

static void sbd_request(struct request_queue *q);

/* Our request queue. */
static struct request_queue *Queue;

static int sbd_open(struct block_device *bd, fmode_t pos)
{
	return 0;
}

/* The device operations structure.*/
static struct block_device_operations sbd_ops = {
	.owner = THIS_MODULE,
	.open = sbd_open,
};

static void sbd_request(struct request_queue *q)
{
}

static int __init sbd_init(void)
{

	size = nsectors * hardsect_size;

	data = vmalloc(size);
	if (data == NULL)
		return -ENOMEM;

	spin_lock_init(&lock);
	Queue = blk_init_queue(sbd_request, &lock);
	if (Queue == NULL)
		return -ENOMEM;

	blk_queue_logical_block_size(Queue, hardsect_size);

	major_num = register_blkdev(major_num, "sbd0");
	if (major_num <= 0) {
		pr_err("sbd: unable to get major number\n");
		return -EIO;
	}
	gd = alloc_disk(1);
	if (!gd)
		return -ENOMEM;

	gd->major = major_num;
	gd->first_minor = 0;
	gd->fops = &sbd_ops;
	strcpy(gd->disk_name, "sbd0");
	set_capacity(gd, size);
	gd->queue = Queue;
	add_disk(gd);
	return 0;
}

static void __exit sbd_exit(void)
{
	del_gendisk(gd);
	put_disk(gd);
	unregister_blkdev(major_num, "sbd0");
	blk_cleanup_queue(Queue);
	vfree(data);
}

module_init(sbd_init);
module_exit(sbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("LKD_CW: block driver ramdisk");
