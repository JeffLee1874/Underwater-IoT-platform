obj-m += vmac_modules.o

vmac_modules-objs := vmac.o mac_contral.o trace.o mac/mac_simple.o

KDIR := /lib/modules/$(shell uname -r)/build/
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
