/**
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/delay.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/completion.h>
#include <linux/crc32.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/irq.h>

#define DRV_NAME "Pci skel"
#define VERSION  "0.9.28"
#define DRIVER_NAME "A RealTek RTL-8139 Fast Ethernet with "  DRV_NAME " of Version : " VERSION


struct drv_private {
	struct pci_dev          	*pdev;			/* pci device */
        void __iomem 			*iobase;		/* pci dev addr */
        struct net_device 		*dev;			/* net device */
        struct net_device_stats 	stats;			/* net status */
	struct napi_struct		napi;			/* napi struct */		
        int 				irq;			/* irq num */
	unsigned char 			*rx_ring;		/* rx ring (page+off) */
        unsigned int 			cur_rx;  		/* curr pkt off in rxring */
        unsigned int 			tx_flag;		/* tx flag */
        unsigned long 			cur_tx;			/* curr pkt off in txring */
        unsigned long 			dirty_tx;		/* dirty pkt off in txring */
        unsigned char 			*tx_buf[NUM_TX_DESC];   /* Tx bounce buffers */
        unsigned char 			*tx_bufs; 		/* tx ring (page+off) */
        dma_addr_t 			rx_ring_dma;		/* rx ring (bus+off) */	
        dma_addr_t 			tx_bufs_dma;		/* tx ring (bus+off) */
        spinlock_t 			lock;			/* tx lock */
        spinlock_t 			rx_lock;		/* rx lock */
        u32 				rx_config;		/* rx config bit */
        unsigned int 			regs_len;		/* reg length */
	u32 				msg_enable;		/* msg enabled bit */
        unsigned long 			fifo_copy_timeout;	/* FIFO time out in tx ring */	
};


/**
 * Probe: Drivers init, called when device is found 
 */
static int __devinit drv_init (struct pci_dev *pdev,
                                       const struct pci_device_id *ent)
{
 	struct net_device 		*dev = NULL;
        struct drv_private 		*np;
	unsigned long                   mmio_start, mmio_end, mmio_flags, mmio_len;
        void __iomem                    *memio;
	int 				disable_dev_on_err = 0,rc,i;

        /* enable pci device */
        rc = pci_enable_device (pdev);
        if (rc)
                goto err_out;
	
	/* discover the resources ie base addr IO(port space) */
        mmio_start      = pci_resource_start (pdev, 1);
        mmio_end        = pci_resource_end (pdev, 1);
        mmio_flags      = pci_resource_flags (pdev, 1);
        mmio_len        = pci_resource_len (pdev, 1);

        pr_info( "\nMMIO region size == 0x%02lX\n", mmio_len);

        /* Request ownership to access address region from pci-core
         */
        rc = pci_request_regions (pdev, DRV_NAME);
        if (rc)
                goto  err_out;

        /* enable PCI bus-mastering */
        pci_set_master(pdev);

        /* ioremap MMIO region */
         memio = pci_iomap(pdev, 1, 0);
        if (memio == NULL) {
	         dev_err(&pdev->dev, "cannot remap MMIO, aborting\n");
                rc = -EIO;
                goto err_out;
        }

		return 0;
}

/**
 * drv_exit - invokded when  device is detached
 *
 */

static void __devexit drv_exit(struct pci_dev *pdev)
{
        pci_disable_device (pdev);
	
}


static struct pci_device_id dev_table[] = {
        {0x10ec, 0x8139, PCI_ANY_ID, PCI_ANY_ID},
	{0,} // end of the list
};

/* pci driver ops */
static struct pci_driver rtl_driver  = {
        .name                   = DRV_NAME,
        .id_table               = dev_table,
        .probe                  = drv_init,
        .remove                 = drv_exit,
};

/* driver init */
static int __init mod_init(void)
{
        pr_info("Registering " DRIVER_NAME "\n");
        return pci_register_driver(&rtl_driver);
}

/* driver cleanup */
static void __exit mod_cleanup(void)
{
        pr_info( "UnRegistering " DRIVER_NAME "\n");
        pci_unregister_driver(&rtl_driver);
}

module_init(mod_init);
module_exit(mod_cleanup);

MODULE_AUTHOR ("TEAM VEDA");
MODULE_DESCRIPTION ("PCI skeleton");
MODULE_LICENSE("GPL");

