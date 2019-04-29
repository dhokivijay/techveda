#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>

#define IOAPIC_BASE 0xFEC00000

void *io;
int init_module(void)
{
	int i, j, maxirq, ident, versn;
	unsigned int val_lo, val_hi;
	void *ioregsel, *iowin;

	int ioredtlb[] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
		0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
		0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21,
		0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
		0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,
		0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
	};

	/* 
	   ioremap_nocache - remaps a physical address range into the 
	   processor's virtual address space, making
	   it available to the kernel
	   IOAPIC_BASE: Physical address of IOAPIC
	   PAZE_SIZE: size of the resource to map 
	 */
	io = ioremap_nocache(IOAPIC_BASE, PAGE_SIZE);

	/* 
	   As per IOAPIC Datasheet 0x00 is I/O REGISTER SELECT  
	   of size 32 bits
	 */
	ioregsel = (void *)((long)io + 0x00);

	/* 
	   As per IOAPIC Datasheet 0x10 is I/O WINDOW REGISTER of size
	   32 bits 
	 */
	iowin = (void *)((long)io + 0x10);

	printk("\n  I/O APIC       ");
	/* Read IOAPIC IDENTIFICATION 
	   As per IOAPIC Datasheet IOAPIC IDENTIFICATION REGISTER 
	   Address Offset : 0x00 

	   IOAPIC IDENTIFICATION REGISTER

	   Bits                 Description
	   ********************************
	   31:28                Reserved        

	   27:24                This 4 bit field contains the IOAPIC 
	   			identification.      

	   23:0                 Reserved        
	 */
	iowrite32(0, ioregsel);
	ident = ioread32(iowin);
	printk(" Identification: %08X  ", ident);

	/* Read IOAPIC VERSION
	   As per OAPIC Datasheet IOAPIC VERSION REGISTER 
	   Address Offset : 0x01 

	   IOAPIC VERSION REGISTER

	   Bits                 Description
	   ********************************
	   31:24                Reserved        

	   23:16                This field contains number of interrupt
	   			input pins for the IOAPIC minus one.

	   15:8                 Reserved        

	   7:0                  This 8 bit field identifies the implementation 
	   			version.
	 */
	iowrite32(1, ioregsel);
	versn = ioread32(iowin);
	printk("     Version: %08X   \n", versn);

	/* mask rest and access bit 16-23 */
	maxirq = (versn >> 16) & 0x00FF;
	maxirq = maxirq + 1;

	printk("\n%25s", " ");
	printk("%d Redirection-Table entries \n", maxirq);

	for (i = 0, j = 0; i < maxirq; i++, j++) {
		iowrite32(ioredtlb[j], ioregsel);
		val_lo = ioread32(iowin);

		j++;
		iowrite32(ioredtlb[j], ioregsel);
		val_hi = ioread32(iowin);

		if ((i % 3) == 0)
			printk("\n");
		printk("  0x%02X : ", i);
		printk("%08X%08X  ", val_hi, val_lo);
	}

	return 0;

}

void cleanup_module(void)
{
	iounmap(io);
}

MODULE_LICENSE("GPL");
