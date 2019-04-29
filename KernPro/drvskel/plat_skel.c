#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/crc32.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/dm9000.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/slab.h>

#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/io.h>


#define DM9000_PHY	0x40	/* PHY address 0x01 */
#define CARDNAME	"dm9000"
#define DRV_VERSION	"1.31"

static int __devinit dm9000_probe(struct platform_device *pdev)
{
	struct dm9000_plat_data *pdata = pdev->dev.platform_data;
	return ret;
}

static int __devexit dm9000_remove(struct platform_device *pdev)
{
        return 0;
}

/* platform driver ops */
static struct platform_driver dm9000_driver = {
	.driver	= {
		.name    = "dm9000", // has to match with name string of corresponding  				     // platform_device 
					
		.owner	 = THIS_MODULE,
		.pm	 = &dm9000_drv_pm_ops,
	},
	.probe   = dm9000_probe,
	.remove  = dm9000_remove,
};

/* platform driver init */
static int __init dm9000_init(void)
{
	return platform_driver_register(&dm9000_driver);
}

/* platform driver cleanup */
static void __exit dm9000_cleanup(void)
{
	platform_driver_unregister(&dm9000_driver);
}

module_init(dm9000_init);
module_exit(dm9000_cleanup);

MODULE_AUTHOR ("TEAM VEDA");
MODULE_DESCRIPTION("Davicom DM9000 network driver");
MODULE_LICENSE("GPL");
