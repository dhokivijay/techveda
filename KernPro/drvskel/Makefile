obj-m := rtc.o
obj-m += apic.o
obj-m += usb_skel.o
all:
	make -C /lib/modules/$(shell uname -r)/build SUBDIRS=$(PWD) modules

install:
	
	make -C /lib/modules/$(shell uname -r)/build SUBDIRS=$(PWD) modules_install

clean:
	make -C /lib/modules/$(shell uname -r)/build SUBDIRS=$(PWD) clean

