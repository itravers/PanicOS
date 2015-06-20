# Author : Isaac Assegai
# Date   : 6/1/2015
# About  : Uses make to build the PanicOS kernel.
#################################################
HOST?=$(shell ../default-host.sh)
HOSTARCH:=$(shell ../target-triplet-to-arch.sh $(HOST))

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_myos_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR:=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

ALL_OUR_OBJS:=\
$(OBJS) \

OBJ_LINK_LIST:=\
$(OBJS) \

all: PanicOS.kernel

.PHONY: all clean install install-headers install-kernel

PanicOS.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/link.ld
	$(CC) -T $(ARCHDIR)/link.ld -o $@ $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
#	nasm -f elf -o $(OBJS)
#	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.asm
	nasm -f elf -o $@ $<

clean:
	rm -f PanicOS.kernel $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: PanicOS.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp PanicOS.kernel $(DESTDIR)$(BOOTDIR)
