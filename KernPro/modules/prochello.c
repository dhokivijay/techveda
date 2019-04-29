#include <linux/module.h>	//kmod interface
#include <linux/version.h>	// kernel version stamp
#include <linux/kernel.h>	// resolve kernel symbol calls
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/init.h>

static struct proc_dir_entry *newfile;
char *filebuf;
int count = 10;

int newfread(char *page, char **start, off_t off,
                          int count, int *eof, void *data)
{
	return sprintf(page, "%s\n", count);
}

int newfwrite(struct file *file, const char __user *buffer,
                           unsigned long count, void *data)
{
	filebuf = krealloc(filebuf, count , GFP_KERNEL);
	copy_from_user(filebuf, buffer, count);
	return 0;
}


int init_module (void)
{
	pr_info (KERN_INFO "module inserted ");
	//newfile = create_proc_entry("newfile", 0664,S_IRUGO | S_IWUSR, NULL);
	newfile = create_proc_entry("newfile", S_IRUGO | S_IWUSR, 0,NULL);
	if(!newfile){
		pr_err("entry failed\n");
		return -1;
	}
	newfile -> read_proc = newfread;
	//newfile -> write_proc = newfwrite;
	newfile -> write_proc = newfwrite;
	newfile -> data = "hello";

	filebuf = kmalloc(4096, GFP_KERNEL);
	strcpy (filebuf, "hello proc");
	return 0;
}

void cleanup_module (void)
{
	kfree (filebuf);
	remove_proc_entry("newfile", NULL);
	pr_info("module removed ");
}

/*Kernel module Comments*/
MODULE_AUTHOR ("support@techveda.org");
MODULE_DESCRIPTION ("LKD_CW: procfs test");
MODULE_LICENSE ("GPL");

