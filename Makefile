obj-m += ABC.o
ABC-objs := hello.o phy_switcher.o vnode.o vport.o simulation/simulation.o

KERNELBUILD := /lib/modules/$(shell uname -r)/build
CURRENT_PATH := $(shell pwd)

all:
	make -C $(KERNELBUILD) M=$(CURRENT_PATH) modules  

clean:
	make -C $(KERNELBUILD) M=$(CURRENT_PATH) clean  
