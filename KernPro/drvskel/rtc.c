#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <asm/io.h>
#include "rtc.h"

static unsigned char get_rtc(unsigned char addr)
{
	outb(addr, ADDRESS_REG);
	return inb(DATA_REG);
}

/*static void set_rtc(unsigned char data, unsigned char addr)
{
	
	outb(addr, ADDRESS_REG);
	outb(data, DATA_REG);
}*/

static int __init rtc_init(void)
{

	pr_info("rtc module loaded\n");	

	pr_info("second %x\n", get_rtc(SECOND));
	pr_info("minute %x\n", get_rtc(MINUTE));
	pr_info("hour %x\n", get_rtc(HOUR));
	pr_info("day %x\n", get_rtc(DAY));
	pr_info("month %x\n", get_rtc(MONTH));
	pr_info("year %x\n", get_rtc(YEAR));

	return 0;
		
}

static void __exit rtc_exit(void)
{
	pr_info("rtc module unloaded\n");
}

module_init(rtc_init);
module_exit(rtc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("CW: Interaction with port mapped address space");
