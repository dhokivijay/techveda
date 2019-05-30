#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>

#include "rtc.h"

static DEFINE_MUTEX(mymtx);

static unsigned char get_rtc(unsigned char addr)
{
	int ret;
	unsigned char c;
	ret = mutex_lock_killable(&mymtx);
	if (ret < 0) {
		pr_err("interrupted while waiting for mutex\n");
		return -1;
	}
	outb(addr, ADDRESS_REG);
	c = inb(DATA_REG);
	mutex_unlock(&mymtx);
	return c;
}

static int set_rtc(unsigned char data, unsigned char addr)
{

	int ret;
	ret = mutex_lock_killable(&mymtx);
	if (ret < 0) {
		pr_err("interrupted while waiting for mutex\n");
		return -EINTR;
	}
	outb(addr, ADDRESS_REG);
	outb(data, DATA_REG);
	mutex_unlock(&mymtx);
	return 0;
}

/* read routine for time entry */
static ssize_t tm_show(struct kobject *kobj, struct kobj_attribute *attr,
		       char *buf)
{
	int cnt;
	struct rtc_time time = { 0 };

	pr_info("%s: Invoked\n", __func__);

	time.sec = get_rtc(SECOND);
	time.min = get_rtc(MINUTE);
	time.hour = get_rtc(HOUR);

	//Copying aquired data to "/sys/time" entry
	cnt =
	    sprintf(buf, "time: %x:%02x:%02x\n", time.hour, time.min, time.sec);
	return cnt;

}

/* write routine for time entry */
static ssize_t tm_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	struct rtc_time time = { 0 };

	pr_info("%s: Invoked\n", __func__);

	sscanf(buf, "time: %x:%x:%x", &time.hour, &time.min, &time.sec);
	set_rtc(time.sec, SECOND);
	set_rtc(time.min, MINUTE);
	set_rtc(time.hour, HOUR);

	return count;

}

/* read routine for date entry */
static ssize_t dt_show(struct kobject *kobj, struct kobj_attribute *attr,
		       char *buf)
{
	int ret;
	struct rtc_time time = { 0 };

	pr_info("%s: Invoked\n", __func__);

	time.day = get_rtc(DAY);
	time.mon = get_rtc(MONTH);
	time.year = get_rtc(YEAR);

	//Copying acquired data to "/sys/date" entry
	ret =
	    sprintf(buf, "date: %x/%02x/20%02x\n", time.day, time.mon,
		    time.year);

	return ret;

}

/* write routine for date entry */
static ssize_t dt_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	struct rtc_time time = { 0 };

	pr_info("%s: Invoked\n", __func__);

	sscanf(buf, "date: %x/%02x/%02x", &time.day, &time.mon, &time.year);
	set_rtc(time.day, DAY);
	set_rtc(time.mon, MONTH);
	set_rtc(time.year, YEAR);

	return count;

}

/* Linking routines to particular entry */
/* Use __ATTR family to ensure that naming convention */


static struct kobj_attribute tm_attribute =
__ATTR(time, 0666, tm_show, tm_store);
static struct kobj_attribute dt_attribute =
__ATTR(date, 0666, dt_show, dt_store);


static struct attribute *attrs[] = {
	&tm_attribute.attr,
	&dt_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *my_rtc;

static int __init rtc_init(void)
{
	int retval;

	pr_info("%s: Invoked\n", __func__);

	/*
	 * kobject_create_and_add - create a struct kobject dynamically and 
           register it with sysfs
	 */
	my_rtc = kobject_create_and_add("my_rtc", NULL /*kernel_kobj */ );
	/*
	 * If you pass kernel_kobj or NULL as 2nd argument entry will
	 * created in /sys/kernel/<kobj-name> or /sys/<kobj-name>
	 */
	if (!my_rtc)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(my_rtc, &attr_group);
	if (retval)
		kobject_put(my_rtc);

	return retval;
}

static void __exit rtc_exit(void)
{
	pr_info("%s: Invoked\n", __func__);
	/* Removing sysfs entry */
	kobject_put(my_rtc);
}

module_init(rtc_init);
module_exit(rtc_exit);

MODULE_LICENSE("GPL");
