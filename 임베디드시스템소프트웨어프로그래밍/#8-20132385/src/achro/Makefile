obj-m := fnd_driver.o
KDIR := /work/kernel
PWD := $(shell pwd)
all: driver app
driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
app:
	arm-none-linux-gnueabi-gcc -static -o fnd_test fnd_test.c
install:
	cp -a fnd_driver.ko /nfsroot
	cp -a fnd_test /nfsroot
clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf *.o
	rm -rf *.order
	rm -rf *.symvers
	rm -rf fnd_test
