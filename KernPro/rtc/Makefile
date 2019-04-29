obj-m:= rtc_sysfs.o

KDIR=/lib/modules/$(shell uname -r)/build

all:	
		
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules  	
	@#insmod rtc_sysfs.ko 	

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	@#rmmod rtc_sysfs #to automate the job of removing the module
