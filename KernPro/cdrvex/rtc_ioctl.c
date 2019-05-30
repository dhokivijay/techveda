#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include "rtc.h"

static unsigned char get_rtc(unsigned char addr)
{
	outb(addr, ADDRESS_REG);
	return inb(DATA_REG);
}

static void set_rtc(unsigned char data, unsigned char addr)
{

	outb(addr, ADDRESS_REG);
	outb(data, DATA_REG);
}

static int rtc_open(struct inode *rtc_inode, struct file *file)
{
	pr_info("rtc node opened\n");
	return 0;
}

static int rtc_close(struct inode *rtc_inode, struct file *file)
{
	pr_info("rtc node closed\n");
	return 0;
}

static ssize_t
rtc_read(struct file *file, char __user * buf, size_t cnt, loff_t * f_pos)
{
	unsigned int ret;
	struct rtc_time time = { 0 };

	if (cnt != sizeof(struct rtc_time)) {
		pr_err("invalid request");
		return -EINVAL;
	}
	pr_info("rtc read invoked\n");

	time.sec = get_rtc(SECOND);
	time.min = get_rtc(MINUTE);
	time.hour = get_rtc(HOUR);
	time.day = get_rtc(DAY);
	time.mon = get_rtc(MONTH);
	time.year = get_rtc(YEAR);

	ret = copy_to_user(buf, &time, sizeof(time));

	return cnt;
}
/* ioctl ---> sys_ioctl---> unlocked_ioctl(file_operations) */
static long rtc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	unsigned char data = arg;

	pr_info("rtc ioctl\n");

/*	if (_IOC_TYPE(cmd) != VEDA_MAGIC)
		return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		if (!access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd)))
			return -EFAULT;

	if (_IOC_DIR(cmd) & _IOC_WRITE)
		if (!access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd)))
			return -EFAULT;   */

	/* sounds ok lets continue */
	switch (cmd) {

	case SET_SECOND:
		set_rtc(data, SECOND);
		break;
	case SET_MINUTE:
		set_rtc(data, MINUTE);
		break;
	case SET_HOUR:
		set_rtc(data, HOUR);
		break;
	case SET_DAY:
		set_rtc(data, DAY);
		break;
	case SET_MONTH:
		set_rtc(data, MONTH);
		break;
	case SET_YEAR:
		set_rtc(data, YEAR);
		break;
	}

	return 0;
}

static struct file_operations rtc_ops = {
	.open = rtc_open,
	.release = rtc_close,
	.read = rtc_read,
//	.write = rtc_write,
	.unlocked_ioctl = rtc_ioctl,
};

static struct miscdevice RtcMisc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEV_NAME,
	.fops = &rtc_ops,
};

static int __init rtc_init(void)
{
	int ret;
	ret = misc_register(&RtcMisc);
	if (ret < 0) {
		pr_err("failed registering with minor %d", RtcMisc.minor);
		return ret;
	}
	pr_info("driver registered with Minor %d", RtcMisc.minor);

	return SUCCESS;
}

static void __exit rtc_exit(void)
{
	misc_deregister(&RtcMisc);

}

module_init(rtc_init);
module_exit(rtc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("LKD_CW: sample char driver for cmos realtime clock");
