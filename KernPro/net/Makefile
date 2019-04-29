obj-m :=netdevices.o
obj-m +=pci.o
obj-m +=netdevstub.o
#obj-m += netdevtransmit.o


KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko  Mo* *.mod.c )
