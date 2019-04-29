
#obj-m +=kb_intr.o
obj-m +=kb_intr1.o
#obj-m +=uiostub.o

KSRC=/lib/modules/$(shell uname -r)/build/

all:
	$(MAKE) -C $(KSRC) M=$(PWD) modules
clean:
	$(MAKE) -C $(KSRC) M=$(PWD) clean
