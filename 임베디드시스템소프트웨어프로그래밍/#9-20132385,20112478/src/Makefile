obj-m := fpga_text_lcd_driver.o

KDIR :=/work/kernel
PWD :=$(shell pwd)
APP := fpga_test_text_lcd
APP2 := test2

all: driver app app2
#all: driver

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
	arm-none-linux-gnueabi-gcc -static -o $(APP) $(APP).c
app2:
	arm-none-linux-gnueabi-gcc -static -o $(APP2) $(APP2).c


install:
	cp -a fpga_text_lcd_driver.ko /nfsroot
	cp -a $(APP) /nfsroot

clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf *.o
	rm -rf $(APP)
	rm -rf Module.symvers
	rm -rf modules.order
	rm -rf .tmp*
	rm -rf .fpga*
