# target & input files

obj-m :=kthread.o 
obj-m +=kthread_atomic.o
obj-m +=kthread_mutex.o
obj-m +=kthread_spin.o
obj-m +=kthread_sync.o
obj-m +=kthread_sync_waitq.o

# variable/macro to hold kernel headers directory path
KDIR= /lib/modules/$(shell uname -r)/build

# command to convert input files into desired target 
# this command uses kbuild scripts found in kernel header folder 
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
