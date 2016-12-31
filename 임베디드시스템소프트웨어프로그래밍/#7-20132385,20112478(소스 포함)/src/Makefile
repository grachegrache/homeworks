obj-m := fpga_led_driver.o
CC := arm-none-linux-gnueabi-gcc
KDIR := /work/kernel
PWD := $(shell pwd)

all: driver app 

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm
app:
	$(CC) -static -o fpga_test_led fpga_test_led.c 
	$(CC) -static -o prog1 prog1.c 
	$(CC) -static -o prog2 prog2.c 
install:
	cp -a fpga_led_driver.ko /nfsroot
	cp -a fpga_test_led /nfsroot
	cp -a prog1 /nfsroot
	cp -a prog2 /nfsroot
clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf *.o
	rm -rf fpga_test_led
	rm -rf Module.symvers
	rm -rf modules.order
	rm -rf .led*
	rm -rf .tmp*
