obj-m:=myhello.o
obj-m+=pagealloc.o
obj-m+=myinterrupt.o
obj-m+=tasklet.o

KDIR=/lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
