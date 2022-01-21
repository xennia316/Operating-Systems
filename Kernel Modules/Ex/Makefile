obj-m += simple.o
KVERSION = $(shell uname -r)
PWD = $(shell pwd)

all: default

default:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean