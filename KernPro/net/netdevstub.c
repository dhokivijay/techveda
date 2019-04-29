/*
 * Building a Basic Network Driver Stub
 *
 * Write a basic network device driver.
 *
 *
 * You should be able to exercise it with:
 *
 *     insmod netdevstub.ko
 *     ifconfig mynet0 up 192.168.3.197
 *     ifconfig mynet0
 *
 * Make sure your chosen address is not being used by anything else.
 *
 * Warning: Depending on kernel version, your stub driver may crash if
 * you try to bring it up or ping it.  If you put in a trivial
 * transmit function, such as
 *
 * static int stub_start_xmit (struct sk_buff *skb, struct net_device *dev)
 * {
 *   dev_kfree_skb (skb);
 *   return 0;
 * }
 * this should avoid the problems.
 *
 @*/
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/init.h>

static struct net_device *dev;

static int my_open(struct net_device *dev)
{
	printk(KERN_INFO "Hit: my_open(%s)\n", dev->name);

	/* start up the transmission queue */

	netif_start_queue(dev);
	return 0;
}

static int my_close(struct net_device *dev)
{
	printk(KERN_INFO "Hit: my_close(%s)\n", dev->name);

	/* shutdown the transmission queue */

	netif_stop_queue(dev);
	return 0;
}

/* Note this method is only needed on some; without it
   module will fail upon removal or use. At any rate there is a memory
   leak whenever you try to send a packet through in any case*/

static int stub_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	dev_kfree_skb(skb);
	return 0;
}

static struct net_device_ops ndo = {
	.ndo_open = my_open,
	.ndo_stop = my_close,
	.ndo_start_xmit = stub_start_xmit,
};

static void my_setup(struct net_device *dev)
{
	int j;
	printk(KERN_INFO "my_setup(%s)\n", dev->name);

	/* Fill in the MAC address with a phoney */

	for (j = 0; j < ETH_ALEN; ++j) {
		dev->dev_addr[j] = (char)j;
	}
/* assign ethernet protocol values to interface protocol specific fields of net_dev structure */
	ether_setup(dev);

	dev->netdev_ops = &ndo;
}
/*
step 1: Allocate net_device instance
Step 2: initialise net_device with driver routines and device config
step 3: register with common net device layer

*/
static int __init my_init(void)
{
	printk(KERN_INFO "Loading stub network module:....");
 /* step 1 & step 2 */
	dev = alloc_netdev(0, "mynet%d", my_setup);//my_setup is net_device initialiser function
	if (register_netdev(dev)) {
		printk(KERN_INFO " Failed to register\n");
		free_netdev(dev);
		return -1;
	}
	printk(KERN_INFO "Succeeded in loading %s!\n\n", dev_name(&dev->dev));
	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "Unloading stub network module\n\n");
	unregister_netdev(dev);
	free_netdev(dev);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Team Veda");
MODULE_DESCRIPTION("netdriver skel");
MODULE_LICENSE("GPL v2");
