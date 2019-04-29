/* i2c skeleton 
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>
#include <linux/mod_devicetable.h>
#include <linux/log2.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>


#define EEPROM_SIZE 256 // size of mini2440 eeprom

/*
 * Called by i2c-core when the i2c device with the address
 * 0x50 is found on the adapter.
 *
 * Inputs - client - i2c device pointer
 *          id     - the id that matched 
 * 
 * Return 0 on succefull probing, negative number
 * on failure
 */
static int eeprom_probe(struct i2c_client *client,
                        const struct i2c_device_id *id)
{

	pr_info("probe invoked \n");
        return 0;
}

/*Will be called from the i2c-core when our eeprom disappears*/
static int eeprom_remove(struct i2c_client *client)
{
	return 0;
}

/*seek Access to device with address 0x50 and enumerated with string "24c08"*/
static struct i2c_device_id eeprom_ids[] = {
        { "24c08",0x50 }, 
	{ } // end of the list
};


static struct i2c_driver eeprom_driver = {
	.driver = {
		.name = "eeprom",
		.owner = THIS_MODULE,
	},
	.probe = eeprom_probe,
	.remove = eeprom_remove,
	.id_table = eeprom_ids,
};

/*module initialization routine*/
static int __init eeprom_init(void)
{
	/*register with i2c-core*/
	return i2c_add_driver(&eeprom_driver);
}
module_init(eeprom_init);

static void __exit eeprom_exit(void)
{
	i2c_del_driver(&eeprom_driver);
}
module_exit(eeprom_exit);

MODULE_DESCRIPTION("Driver for I2C EEPROM");
MODULE_AUTHOR("Veda Solutions");
MODULE_LICENSE("GPL");
