#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/slab.h>

#define VENDOR 0x154b
#define DEVICE 0x6002
static int
test_probe(struct usb_interface *intf, const struct usb_device_id *id)
{

	struct usb_device *dev = interface_to_usbdev(intf);

	pr_info("\nmy_usb_probe\n");
	pr_info("devnum=%d, speed=%d\n", dev->devnum, (int)dev->speed);
	pr_info("idVendor=0x%hX, idProduct=0x%hX, bcdDevice=0x%hX\n",
		dev->descriptor.idVendor,
		dev->descriptor.idProduct, dev->descriptor.bcdDevice);
	pr_info("class=0x%hX, subclass=0x%hX\n",
		dev->descriptor.bDeviceClass, dev->descriptor.bDeviceSubClass);
	pr_info("protocol=0x%hX, packetsize=%hu\n",
		dev->descriptor.bDeviceProtocol,
		dev->descriptor.bMaxPacketSize0);
	pr_info("manufacturer=0x%hX, product=0x%hX, serial=%hu\n",
		dev->descriptor.iManufacturer, dev->descriptor.iProduct,
		dev->descriptor.iSerialNumber);

	return 0;
}

static void test_disconnect(struct usb_interface *intf)
{
	pr_info("\nmy_usb_disconnect\n");
}

static struct usb_device_id dev_table[] = {
	{USB_DEVICE(VENDOR,DEVICE)},	/* sandisk usb storage device id's */
	{}				/* Null terminator (required) */
};

MODULE_DEVICE_TABLE(usb, dev_table);

static struct usb_driver my_usb_driver = {
	.name = "usb-test",
	.probe = test_probe,  // driver's init call
	.disconnect = test_disconnect, // driver's exit call
	.id_table = dev_table,
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
