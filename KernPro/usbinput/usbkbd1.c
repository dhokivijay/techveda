/*
 * USB Keyboard driver
 * Authors:  Vojtech Pavlik
 *           Team veda
 * Copyright 2012, techveda, www.techveda.org
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/usb.h>

/*
 * Scancode to kernel keycode table, see linux/input.h
 * for all Linux keycodes
 */
static const unsigned char usb_kbd_keycode[256] = {
	  0,  0,  0,  0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38,
	 50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,  2,  3,
	  4,  5,  6,  7,  8,  9, 10, 11, 28,  1, 14, 15, 57, 12, 13, 26,
	 27, 43, 43, 39, 40, 41, 51, 52, 53, 58, 59, 60, 61, 62, 63, 64,
	 65, 66, 67, 68, 87, 88, 99, 70,119,110,102,104,111,107,109,106,
	105,108,103, 69, 98, 55, 74, 78, 96, 79, 80, 81, 75, 76, 77, 71,
	 72, 73, 82, 83, 86,127,116,117,183,184,185,186,187,188,189,190,
	191,192,193,194,134,138,130,132,128,129,131,137,133,135,136,113,
	115,114,  0,  0,  0,121,  0, 89, 93,124, 92, 94, 95,  0,  0,  0,
	122,123, 90, 91, 85,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 29, 42, 56,125, 97, 54,100,126,164,166,165,163,161,115,114,113,
	150,158,159,128,136,177,178,176,142,152,173,140
};


/**
 * struct usb_kbd - state of each attached keyboard
 * @dev:	input device associated with this keyboard
 * @usbdev:	usb device associated with this keyboard
 * @old:	data received in the past from the @irq URB representing which
 *		keys were pressed. By comparing with the current list of keys
 *		that are pressed, we are able to see key releases.
 * @irq:	URB for receiving a list of keys that are pressed when a
 *		new key is pressed or a key that was pressed is released.
 * @name:	Name of the keyboard. @dev's name field points to this buffer
 * @phys:	Physical path of the keyboard. @dev's phys field points to this
 *		buffer
 * @new:	Buffer for the @irq URB
 * @new_dma:	DMA address for @irq URB
 */
struct usb_kbd {
	struct input_dev *dev;
	struct usb_device *usbdev;
	unsigned char old[8];
	struct urb *irq;
	char name[128];
	char phys[64];

	unsigned char *new;
	dma_addr_t new_dma;
};

/*
 * usb_kbd_irq: We have submited Interrupt URB in the usb_kbd_open(), once
 * the URB has been processed this call back is called. We will submit the 
 * URB again.
 */

static void usb_kbd_irq(struct urb *urb)
{
	 /*
         * Get our private object, stored in URB context with 
         * usb_fill_int_urb() in usb_kbd_probe().
         */
	struct usb_kbd *kbd = urb->context;
	int i;

	switch (urb->status) {
	case 0:			/* success */
		break;
	case -ECONNRESET:	/* unlink */
	case -ENOENT:
	case -ESHUTDOWN:
		return;
	/* -EPIPE:  should clear the halt */
	default:		/* error */
		goto resubmit;
	}
	
        /*
         * Request was success. Device returns 8 bytes of data
         * in an array. This is called INPUT report. As the
         * report descriptor defines, the INPUT report is formatted
	 * as follows;
         * one byte of modifier byte and 6 bytes keycode array.
         *
  	     byte
              0     Modifier byte
    	      1     reserved
    	      2     keycode array (0)
    	      3     keycode array (1)
    	      4     keycode array (2)
    	      5     keycode array (3)
    	      6     keycode array (4)
    	      7     keycode array (5)

	 * The bitmap of modifier byte is defined like this

    	  bit
    	   0     LEFT CTRL
    	   1     LEFT SHIFT
           2     LEFT ALT
           3     LEFT GUI
           4     RIGHT CTRL
           5     RIGHT SHIFT
           6     RIGHT ALT
           7     RIGHT GUI

	 * The keycode array can hold up to 6 keys which are
         * simultaneously pushed. The order of keycode in the array is arbitrary.
         */
	
	/*First send the modifier byte status to input subsystem*/
	for (i = 0; i < 8; i++)
		input_report_key(kbd->dev, usb_kbd_keycode[i + 224], (kbd->new[0] >> i) & 1);

	/* Now send up the keycode*/
	for (i = 2; i < 8; i++) {
		/*
		 * Check if the old key is in the new keys list, if not, send key
		 * release signal to input subsystem
		 */
		if (kbd->old[i] > 3 && memscan(kbd->new + 2, kbd->old[i], 6) == kbd->new + 8) {
			if (usb_kbd_keycode[kbd->old[i]])
				input_report_key(kbd->dev, usb_kbd_keycode[kbd->old[i]], 0);
		}
		/*
		 * Check if the new key is in the old keys list, if not send key pressed 
		 * signal to the input subsystem
		 */
		if (kbd->new[i] > 3 && memscan(kbd->old + 2, kbd->new[i], 6) == kbd->old + 8) {
			if (usb_kbd_keycode[kbd->new[i]])
				input_report_key(kbd->dev, usb_kbd_keycode[kbd->new[i]], 1);
		}
	}

	/* Tell input subsystem to push up the keys*/
	input_sync(kbd->dev);

	/*backup current key info*/
	memcpy(kbd->old, kbd->new, 8);

resubmit:
	/*Now resubmit the URB */
	i = usb_submit_urb (urb, GFP_ATOMIC);
	if (i)
		pr_info("can't resubmit intr, %s-%s/input0, status %d",
			kbd->usbdev->bus->bus_name,
			kbd->usbdev->devpath, i);
	
	
}


/*
 * Set interrupt URB device and submit the urb. Once the request is
 *  data our usb_kbd_irq() call back function is called.
 */
static int usb_kbd_open(struct input_dev *dev)
{
	struct usb_kbd *kbd = input_get_drvdata(dev);

	kbd->irq->dev = kbd->usbdev;
	if (usb_submit_urb(kbd->irq, GFP_KERNEL))
		return -EIO;

	return 0;
}

static void usb_kbd_close(struct input_dev *dev)
{
	struct usb_kbd *kbd = input_get_drvdata(dev);

	usb_kill_urb(kbd->irq);
}
/*
 * usb_kbd_alloc_mem: Allocate resources like urb and buffers
 */
static int usb_kbd_alloc_mem(struct usb_device *dev, struct usb_kbd *kbd)
{
        /*
         * Allocate URB for Interrupt IN
         */
	if (!(kbd->irq = usb_alloc_urb(0, GFP_KERNEL)))
		return -1;
	 /*
         * Allcate buffer to store read data
         */
	if (!(kbd->new = usb_alloc_coherent(dev, 8, GFP_ATOMIC, &kbd->new_dma)))
		return -1;

	return 0;
}

static void usb_kbd_free_mem(struct usb_device *dev, struct usb_kbd *kbd)
{
	/*
	 * usb_kbd_free_mem: Free resources like URB and buffers.
 	 */
	usb_free_urb(kbd->irq);
	usb_free_coherent(dev, 8, kbd->new, kbd->new_dma);
}

/*
 * usb_kbd_probe: Called by core when the usb keyboard is plugged in.
 * Inputs:      iface   - usb interface object we are interested in.
 *              id      - Table of devices that match our request.
 *
 * Step1: Get All end points we are interested in.
 * Step2: Allocate All required urbs and memory
 * Step3: Register with Input subsystem
 */ 	
static int usb_kbd_probe(struct usb_interface *iface,
			 const struct usb_device_id *id)
{
	/*
         * Get usb_device object where this interface is on.
         */
	struct usb_device *dev = interface_to_usbdev(iface);
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor *endpoint;
	struct usb_kbd *kbd; // our private structure.
	struct input_dev *input_dev; // to register with input subsystem
	int i, pipe, maxp;
	int error = -ENOMEM;

	interface = iface->cur_altsetting;

	/*
         * We dont support more than one endpoint. We support interrupt In 
         * endpoint along with default controll endpoint.
         */
	if (interface->desc.bNumEndpoints != 1)
		return -ENODEV;
	/*
         * Get pointer to the end point descriptor and check if it
         * interrupt In endpoint
         * bEndpointAddress field of 
         * endpoint descriptor is like this.
         *
         * Bit 3...0: The endpoint number
         * Bit 6...4: Reserved, reset to zero
         * Bit 7:    Direction, ignored for control endpoints
         *           0 = OUT endpoint
         *           1 = IN endpoint
         * To check if this is IN endpoint code looks like this:
         *
         *    if ((endpoint->bEndpointAddress & 0x80)).
         * 
         * bAttributes field of endpoint descriptor is like this:
         *
         * Bits 1..0: Transfer Type
         *            00 = Control
         *            01 = Isochronous
         *            10 = Bulk
         *            11 = Interrupt
         * Bits 3..2: Synchronization Type
         *            00 = No Synchronization
         *            01 = Asynchronous
	 *            10 = Adaptive
         *            11 = Synchronous
         * Bits 5..4: Usage Type
         *            00 = Data endpoint
         *            01 = Feedback endpoint
         *            10 = Implicit feedback Data endpoint
         *            11 = Reserved
         *
         * To Check if an endpoint is Interrupt Endpoint cod looks like:
         *      if ((endpoint->bmAttributes & 3) != 3)
         *
         * Both the above tasks can be done using a single function 
         * usb_endpoint_is_int_in().
         */
	endpoint = &interface->endpoint[0].desc;
	if (!usb_endpoint_is_int_in(endpoint))
		return -ENODEV;

	/*
         * Get interrupt pipe. A pipe is combination of usb_device, interface
         * and endpoint.  
         */
	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
	/*
         * Get the max packet size that we can deal with this endpoint. returns
         * wMaxPacketSize field of endpoint interface descriptor.
         */
	maxp = usb_maxpacket(dev, pipe, usb_pipeout(pipe));
	/*
         * Allocate memory for our private structure and initialize with zero.
         */
	kbd = kzalloc(sizeof(struct usb_kbd), GFP_KERNEL);
	/*
	 * Allocate struct input_dev object, this object will be given
	 * to the input subsystem
	 */
	input_dev = input_allocate_device();
	if (!kbd || !input_dev)
		goto fail1;
	/*
         * Allocate remaining resources like urbs
         */
	if (usb_kbd_alloc_mem(dev, kbd))
		goto fail2;
        /*      
         * Store ref to usb_dev and input_dev in the private object
         */

	kbd->usbdev = dev;
	kbd->dev = input_dev;
	
	/*Get the keyboard manufacturer id and product id*/
	if (dev->manufacturer)
		strlcpy(kbd->name, dev->manufacturer, sizeof(kbd->name));

	if (dev->product) {
		if (dev->manufacturer)
			strlcat(kbd->name, " ", sizeof(kbd->name));
		strlcat(kbd->name, dev->product, sizeof(kbd->name));
	}

	if (!strlen(kbd->name))
		snprintf(kbd->name, sizeof(kbd->name),
			 "USB HIDBP Keyboard %04x:%04x",
			 le16_to_cpu(dev->descriptor.idVendor),
			 le16_to_cpu(dev->descriptor.idProduct));

	usb_make_path(dev, kbd->phys, sizeof(kbd->phys));
	strlcat(kbd->phys, "/input0", sizeof(kbd->phys));
	
	/*Set the input device name, phyp, id parent device*/
	input_dev->name = kbd->name;
	input_dev->phys = kbd->phys;
	usb_to_input_id(dev, &input_dev->id);
	input_dev->dev.parent = &iface->dev;

	/*store out private data pointer in device object*/
	input_set_drvdata(input_dev, kbd);

	/* Tell input subsystem our capabilities*/
	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);

	/*Tell the input subsystem the keys that we handle*/
	for (i = 0; i < 255; i++)
		set_bit(usb_kbd_keycode[i], input_dev->keybit);
	clear_bit(0, input_dev->keybit);

	/*Tell our open and close entry points from the input subsystem*/
	input_dev->open = usb_kbd_open;
	input_dev->close = usb_kbd_close;
	
	/*
         * Fill in the Interrupt URB. Submit it to the usb core in the
         * usb_kbd_open() and interrupt call back usb_kbd_irq(). In URB 
         * context we store our private object so that we can access it in
         * the usb_kbd_irq().
         */
	usb_fill_int_urb(kbd->irq, dev, pipe,
			 kbd->new, (maxp > 8 ? 8 : maxp),
			 usb_kbd_irq, kbd, endpoint->bInterval);
	/*
         * transfer_dma, where received data is kept
         */
	kbd->irq->transfer_dma = kbd->new_dma;
	kbd->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	/*Ok, we are done, tell it to the input subsystem*/
	error = input_register_device(kbd->dev);
	if (error)
		goto fail2;

	usb_set_intfdata(iface, kbd);

	/*tell the user we are ready*/
	pr_info(KERN_INFO "My USB Keyboard: %s is up", kbd->name);
	return 0;

fail2:	
	usb_kbd_free_mem(dev, kbd);
fail1:	
	input_free_device(input_dev);
	kfree(kbd);
	return error;
}

/*
 * usb_kbd_disconnect: Called by USB Core when our keyboard is unplugged or
 * our module is removed.
 */
static void usb_kbd_disconnect(struct usb_interface *intf)
{
	/*
         * Get our driver private structure stored in interface object using 
         * usb_set_intfdata() in usb_kbd_probe().
         */
	struct usb_kbd *kbd = usb_get_intfdata (intf);

	usb_set_intfdata(intf, NULL);
	/*
         * If our usb_kbd_probe() probe function had been called, we need
         * to free the resources allocated. 
         *
         */
	if (kbd) {
		/*Kill the urb if already submitted*/
		usb_kill_urb(kbd->irq);
		/*Unregister from the input subsystem*/
		input_unregister_device(kbd->dev);
		/*Free the memory we have allocated*/ 
		usb_kbd_free_mem(interface_to_usbdev(intf), kbd);
		/*Free our private data*/
		kfree(kbd);
	}
}


/*
 * udb_kbd_id_table: Table that specify what device we are dealing with. We
 * are interrested in Human Interface device(HID). We match our device against 
 * interface descriptor, So we tell usb to search for a device which has class
 * as 3(HID), subclass as 1(boot interface) and protocol as 1(keyboard).
 * refer  4.1, 4.2 and 4.3 sections of USB HID Specification 1.1.
 */
static struct usb_device_id usb_kbd_id_table [] = {
        { USB_INTERFACE_INFO(3, 1, 1 ) },
        { },                                             /* Terminating entry */
};

MODULE_DEVICE_TABLE (usb, usb_kbd_id_table);

static struct usb_driver usb_kbd_driver = {
        .name =         "usbkbd",  // our device name
        .probe =        usb_kbd_probe, // called when our keyboard is found
        .disconnect =   usb_kbd_disconnect, // called when keyboard is removed
        .id_table =     usb_kbd_id_table,
};

/*
 * usb_kbd_init, module initialization routine, register with the USB Core.
 */
static int __init usb_kbd_init(void)
{
        int ret;
	
	ret  = usb_register(&usb_kbd_driver);
        if (ret == 0)
                pr_info("usbkbd: Registered with USB Core");
	return ret;
}

static void __exit usb_kbd_exit(void)
{
        usb_deregister(&usb_kbd_driver);
}

module_init(usb_kbd_init);
module_exit(usb_kbd_exit);


MODULE_AUTHOR("techveda.org");
MODULE_DESCRIPTION("USB Keyboard Driver");
MODULE_LICENSE("GPL");

