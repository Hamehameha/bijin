CC = sh4-linux-gcc

export SYSROOT=$(shell readlink -f `$(CC) -print-prog-name=gcc` | sed -e s!/usr/sh4-linux-uclibc/.*!!)

CFLAGS = -Wall `sh4-linux-directfb-config --cflags`
LDFLAGS = `sh4-linux-directfb-config --libs` -lasound -lts


objs = jpg_viewer


all: $(objs)


taa:
	@echo $(SYSROOT)

clean:
	$(RM) $(objs)
