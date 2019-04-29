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


/*read -> | sys_read-> driv_read() 
  char buf[100];
  read(fd, buf, 100);
*/
/* step 1: validate read request
   step 2: verify if device is ready to be read
   step 3: read data
   step 4: process data if needed
   step 5: transfer data to application buffer
   step 6: return no of bytes transferred
*/
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
/*  write -> sys_write--> driv_write
    char buf[100];
    write(fd, buf, 100);
*/
static ssize_t
rtc_write(struct file *file, const char __user * buf, size_t cnt,
	  loff_t * f_pos)
{

	unsigned int ret;
	 struct rtc_time time = { 0 };

        if (cnt != sizeof(struct rtc_time)) {
                pr_err("invalid request");
                return -EINVAL;
        }

	ret = copy_from_user(&time, buf, sizeof(time));

        set_rtc(time.sec, SECOND);
        set_rtc(time.min, MINUTE);
        set_rtc(time.hour, HOUR);
        set_rtc(time.day, DAY);
        set_rtc(time.mon, MONTH);
        set_rtc(time.year, YEAR);

	return cnt;
}


static struct file_operations rtc_ops = {
	.open = rtc_open,
	.release = rtc_close,
	.read = rtc_read,
	.write = rtc_write,
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
