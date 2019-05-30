#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>

#define  SECOND         0x00	//Second       (00..59)
#define  MINUTE         0x02	//Minute       (00..59)
#define  HOUR           0x04	//Hour         (00..23)
#define  DAY_IN_WEEK    0x06	//Day of week  (01..07)
#define  DAY            0x07	//Day          (01..31)
#define  MONTH          0x08	//Month        (01..12)
#define  YEAR           0x09	// Year         (00..99)

#define  ADDRESS_REG       0x70
#define  DATA_REG          0x71
#define  ADDRESS_REG_MASK  0xe0

#define NODETIME "driver/time"
#define NODEDATE "driver/date"

typedef struct {
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
} rtc_time;

typedef struct {
	unsigned char day;
	unsigned char mon;
	unsigned char year;
} rtc_date;

static struct proc_dir_entry *my_proc;
static struct proc_dir_entry *my_proc1;

static unsigned char rtc_inb(unsigned char addr)
{
	outb(addr, ADDRESS_REG);
	return inb(DATA_REG);
}

static int
read_time(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	rtc_time time;
	time.sec = rtc_inb(SECOND);
	time.min = rtc_inb(MINUTE);
	time.hour = rtc_inb(HOUR);

	return sprintf(page, "%x:%x:%x\n", time.hour, time.min, time.sec);
}

static int
read_date(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	rtc_date date;
	date.day = rtc_inb(DAY);
	date.mon = rtc_inb(MONTH);
	date.year = rtc_inb(YEAR);

	return sprintf(page, "%x:%x:%x\n", date.day, date.mon, date.year);
}

static int __init my_init(void)
{
	my_proc = create_proc_entry(NODETIME, S_IRUGO | S_IWUSR, NULL);
	if (!my_proc) {
		pr_err("I failed to make %s\n", NODETIME);
		return -ENOMEM;
	}
	my_proc1 = create_proc_entry(NODEDATE, S_IRUGO | S_IWUSR, NULL);
	if (!my_proc1) {
		pr_err("I failed to make %s\n", NODEDATE);
		return -ENOMEM;
	}
	my_proc->read_proc = read_time;
	my_proc1->read_proc = read_date;
	return 0;
}

static void __exit my_exit(void)
{
		remove_proc_entry(NODETIME, NULL);
		remove_proc_entry(NODEDATE, NULL);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team Veda");
MODULE_DESCRIPTION("proc files");
MODULE_LICENSE("GPL");
