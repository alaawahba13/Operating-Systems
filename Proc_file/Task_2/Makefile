obj-m += lab1.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
test:
	sudo insmod lab1.ko
	sleep 3s
	cat /proc/seconds
	sudo rmmod lab1	
