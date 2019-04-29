/*
 * Examining Network Devices
 *
 * All network devices are linked together in a list.  You can get a
 * pointer to the head of the list and then *walk through it using:
 *
 *   struct net_device *first_net_device (struct net *net);
 *   struct net_device *next_net_device(struct net_device *dev);
 @*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>

static int __init my_init(void)
{
	struct net_device *dev;
	printk(KERN_INFO "Hello: module loaded at 0x%p\n", my_init);
	dev = first_net_device(&init_net);
	printk(KERN_INFO "Hello: dev_base address=0x%p\n", dev);
	while (dev) {
		printk(KERN_INFO
		       "name = %6s irq=%4d trans_start=%12lu last_rx=%12lu\n",
		       dev->name, dev->irq, dev->trans_start, dev->last_rx);
		dev = next_net_device(dev);
	}
	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "Module Unloading\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team Veda");
MODULE_DESCRIPTION("Net Device list");
MODULE_LICENSE("GPL v2");
