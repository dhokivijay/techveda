obj-m :=myhello.o 
obj-m +=modex.o
obj-m +=moddep1.o
obj-m +=modparam.o
obj-m +=modexfunc.o
obj-m +=tfunc.o
obj-m +=modcounter.o
obj-m +=moddep1.o
obj-m+=prochello.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
