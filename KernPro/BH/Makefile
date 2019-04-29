obj-m := timer2.o
obj-m += wqshared.o
obj-m += wqpriv.o
obj-m += wqnewi.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	@rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c .*cmd .tmp*)
