#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/ch9.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

/* Get a minor range for your devices from the usb maintainer */
#define USB_SKEL_MINOR_BASE     192

/* Structure to hold all of our device specific stuff */
struct drv_priv {
	struct usb_device *udev;		/* the usb device for this device */
	struct usb_interface *interface;	/* the interface for this device */
	unsigned char *bulk_in_buffer;		/* the buffer to receive data */
	struct urb  *bulk_in_urb;               /* the urb to read data with */
	size_t bulk_in_size;			/* the size of the receive buffer */
	size_t bulk_in_read;			/* size of data read from device */
	__u8 bulk_in_endpointAddr;		/* the address of the bulk in endpoint */
	__u8 bulk_out_endpointAddr;		/* the address of the bulk out endpoint */
	int errors;				/* the last request tanked */
        struct completion read_completion;     /* to wait for an ongoing read */
	struct kref kref;
};

#define get_drv_priv(d) container_of(d, struct drv_priv, kref)
static struct usb_driver storage_driver;

static void priv_delete(struct kref *kref)
{
	struct drv_priv *dev = get_drv_priv(kref);
	usb_put_dev(dev->udev);
	kfree(dev->bulk_in_buffer);
	kfree(dev);
}
static int drv_open(struct inode *inode, struct file *file)
{
  	struct drv_priv *dev;
	struct usb_interface *interface;
	int subminor;
	int retval;

	subminor = iminor(inode);
	interface = usb_find_interface(&storage_driver, subminor);
        if (!interface) {
                pr_err("%s - error, can't find device for minor %d\n",
                        __func__, subminor);
                retval = -ENODEV;
                return retval;
        }


	dev = usb_get_intfdata(interface);
        if (!dev) {
                retval = -ENODEV;
		return retval;
        }

	/* increment our usage count for the device */
        kref_get(&dev->kref);
	
	retval = usb_autopm_get_interface(interface);
        if (retval)
                return retval;

	/* save our object in the file's private structure */
        file->private_data = dev;

	return 0;
}

static int drv_release(struct inode *inode, struct file *file)
{
        struct drv_priv *dev;

        dev = file->private_data;
        if (dev == NULL)
                return -ENODEV;

        /* allow the device to be autosuspended */
	if (dev->interface)
        	usb_autopm_put_interface(dev->interface);

        /* decrement the count on our device */
         kref_put(&dev->kref, priv_delete);
        return 0;
}

static void drv_read_bulk_callback(struct urb *urb)
{
        struct drv_priv *dev;

        dev = urb->context;

        if (urb->status) {
                if (!(urb->status == -ENOENT ||
                    urb->status == -ECONNRESET ||
                    urb->status == -ESHUTDOWN))
                        dev_err(&dev->interface->dev,
                                "%s - nonzero write bulk status received: %d\n",
                                __func__, urb->status);

                dev->errors = urb->status;
        } else {
                dev->bulk_in_read = urb->actual_length;
        }
        complete(&dev->read_completion);
}


static ssize_t drv_read(struct file *file, char *buffer, size_t count,
                         loff_t *ppos)
{
        struct drv_priv *dev;
        int rv;

        dev = file->private_data;

        /* if we cannot read at all, return EOF */
        if (!dev->bulk_in_urb || !count)
                return 0;
	
	/* Allocate urb */
	dev->bulk_in_urb = usb_alloc_urb(0, GFP_KERNEL);
                        if (!dev->bulk_in_urb) {
                                dev_err(&dev->udev->dev,
                                        "Could not allocate bulk_in_urb\n");
                                return -ENOMEM;
                        }
	/* Allocate buffer */
	dev->bulk_in_buffer = kmalloc(dev->bulk_in_size, GFP_KERNEL);
                        if (!dev->bulk_in_buffer) {
                                dev_err(&dev->udev->dev,
                                        "Could not allocate bulk_in_buffer\n");
                        }


	/* prepare a read */
        usb_fill_bulk_urb(dev->bulk_in_urb,
                        dev->udev,
                        usb_rcvbulkpipe(dev->udev,
                                dev->bulk_in_endpointAddr),
                        dev->bulk_in_buffer,
                        min(dev->bulk_in_size, count),
                        drv_read_bulk_callback,
                        dev);

        /* submit urb  */
        rv = usb_submit_urb(dev->bulk_in_urb, GFP_KERNEL);
        if (rv < 0) {
                dev_err(&dev->udev->dev,
                        "%s - failed submitting read urb, error %d\n",
                        __func__, rv);
        }
	rv = wait_for_completion_interruptible(&dev->read_completion);
                if (rv < 0)
                        goto exit;
	
                /* data is available
                 */

                if (copy_to_user(buffer,
                                 dev->bulk_in_buffer,
                                 dev->bulk_in_read))
                        rv = -EFAULT;
                else
                        rv = dev->bulk_in_read;
exit:
        	return rv;
}

static void drv_write_bulk_callback(struct urb *urb)
{
        struct drv_priv *dev;

        dev = urb->context;

        if (urb->status) {
                if (!(urb->status == -ENOENT ||
                    urb->status == -ECONNRESET ||
                    urb->status == -ESHUTDOWN))
                        dev_err(&dev->interface->dev,
                                "%s - nonzero write bulk status received: %d\n",
                                __func__, urb->status);

                dev->errors = urb->status;
        }

        /* free up our allocated buffer */
        usb_free_coherent(urb->dev, urb->transfer_buffer_length,
                          urb->transfer_buffer, urb->transfer_dma);
}


static ssize_t drv_write(struct file *file, const char *user_buffer,
                          size_t count, loff_t *ppos)
{
        struct drv_priv *dev;
        int retval = 0;
        struct urb *urb = NULL;
        char *buf = NULL;
        size_t writesize = count;

        dev = file->private_data;

        /* verify that we actually have some data to write */
        if (count == 0)
                goto exit;

        /* create a urb, and a buffer for it, and copy the data to the urb */
        urb = usb_alloc_urb(0, GFP_KERNEL);
        if (!urb) {
                retval = -ENOMEM;
                goto error;
        }
	/* allocate dma coherent buffer to transfer payload */
        buf = usb_alloc_coherent(dev->udev, writesize, GFP_KERNEL,
                                 &urb->transfer_dma);
        if (!buf) {
                retval = -ENOMEM;
                goto error;
        }
	/* write payload(usb device class protocol) into dma buffer */
        if (copy_from_user(buf, user_buffer, writesize)) {
                retval = -EFAULT;
                goto error;
        }

        /* initialize URB with all details */
        usb_fill_bulk_urb(urb, dev->udev,
                          usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
                          buf, writesize, drv_write_bulk_callback, dev);
        
	urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

        /* send the data out the bulk port */
        retval = usb_submit_urb(urb, GFP_KERNEL);
        if (retval) {
                dev_err(&dev->interface->dev,
                        "%s - failed submitting write urb, error %d\n",
                        __func__, retval);
                goto error_unanchor;
        }

        /*
         * release our reference to this urb, the USB core will eventually free
         * it entirely
         */
         usb_free_urb(urb);


        return writesize;

error_unanchor:
        usb_unanchor_urb(urb);
error:
        if (urb) {
                usb_free_coherent(dev->udev, writesize, buf, urb->transfer_dma);
                usb_free_urb(urb);
        }

exit:
        return retval;
}


static const struct file_operations storage_ops = {
        .owner =        THIS_MODULE,
        .read =         drv_read,
        .write =        drv_write,
        .open =         drv_open,
        .release =      drv_release,
};

/*
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with the driver core
 */
static struct usb_class_driver storage_class = {
        .name =         "storage%d",
        .fops =         &storage_ops,		// char drivers file_ops object 
        .minor_base =   USB_SKEL_MINOR_BASE,
};


static int drv_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct drv_priv *dev;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	size_t buffer_size;
	int i, retval;

	/* allocate memory for our device state and initialize it */
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev) {
		dev_err(&intf->dev, "Out of memory\n");
		return -ENOMEM;
	}

	dev->udev = interface_to_usbdev(intf);
	dev->interface = intf;

	/* set up the endpoint information */
	/* use only the first bulk-in and bulk-out endpoints */
	iface_desc = intf->cur_altsetting;
	for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
		endpoint = &iface_desc->endpoint[i].desc;

		if (!dev->bulk_in_endpointAddr &&
		    usb_endpoint_is_bulk_in(endpoint)) {
			/* we found a bulk in endpoint */
			buffer_size = __le16_to_cpu(endpoint->wMaxPacketSize);
			dev->bulk_in_size = buffer_size;
			dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
			}

		if (!dev->bulk_out_endpointAddr &&
		    usb_endpoint_is_bulk_out(endpoint)) {
			/* we found a bulk out endpoint */
			dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
		}
	}
	if (!(dev->bulk_in_endpointAddr && dev->bulk_out_endpointAddr)) {
		dev_err(&intf->dev,
			"Could not find both bulk-in and bulk-out endpoints\n");
		goto error;
	}

	/* save our data pointer in this interface device */
	usb_set_intfdata(intf, dev);

        /* we can register the device now, as it is ready */
        retval = usb_register_dev(intf, &storage_class);
        if (retval) {
                /* something prevented us from registering this driver */
                dev_err(&intf->dev,
                        "Not able to get a minor for this device.\n");
                usb_set_intfdata(intf, NULL);
                goto error;
        }

        /* let the user know what node this device is now attached to */
        dev_info(&intf->dev,
                 "USB Skeleton device now attached to USBSkel-%d",
                 intf->minor);


	return 0;
 error:
	if (dev)
		/* this frees allocated memory */
		kref_put(&dev->kref, priv_delete);
	return -ENOMEM;

}

static void drv_disconnect(struct usb_interface *intf)
{
	struct drv_priv *dev;
	dev = usb_get_intfdata(intf);
	usb_set_intfdata(intf, NULL);
	kref_put(&dev->kref, priv_delete);

	 /* give back our minor */
        usb_deregister_dev(intf, &storage_class);

	dev_info(&intf->dev, "USB Skeleton now disconnected");
}

static struct usb_device_id dev_table[] = {
	{USB_DEVICE(0x0781, 0x5572)},	/* sandisk cruzer */
	{}			/* Null terminator (required) */
};

MODULE_DEVICE_TABLE(usb, dev_table);

static struct usb_driver test_driver = {
	.name = "usb-flashdrive",
	.probe = drv_probe,
	.disconnect = drv_disconnect,
	.id_table = dev_table,
};

int __init init_module(void)
{
	int err;
	pr_info("registering with usb core");
	err = usb_register(&test_driver);
	return err;
}

void __exit cleanup_module(void)
{
	usb_deregister(&test_driver);
}

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("usb storage probe driver");
MODULE_LICENSE("GPL");
