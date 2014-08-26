obj-m += hello_world.o

CFLAGS_hello_world.o := -DDEBUG

all:
	    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
