obj-m :=usbkbd1.o

KDIR= /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* .c* .t* *.markers *.symv* *.order*
