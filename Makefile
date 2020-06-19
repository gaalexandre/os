PREFIX:=$(HOME)/os-dev/cross/bin
CC:=$(PREFIX)/i686-elf-g++
AS:=$(PREFIX)/i686-elf-as
CFLAGS?=-O2
CFLAGS=$(CFLAGS?) -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc
CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

OBJS:=kernel.o terminal.o

OBJ_LINK_LIST:= boot.o $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:= boot.o $(CRTI_OBJ) $(OBJS) $(CRTN_OBJ)

build: iso

kernel: $(OBJ_LINK_LIST)
	$(CC) -T linker.ld -o myos.bin $(OBJ_LINK_LIST) $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
%.o: %.s
	$(AS) $< -o $@
iso: kernel
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

test: build
	grub-file --is-x86-multiboot myos.bin

qemu: build
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -rf isodir $(INTERNAL_OBJS) myos.bin myos.iso GPUCache
