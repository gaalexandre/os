PREFIX:=$(HOME)/os-dev/cross/bin
CC:=$(PREFIX)/i686-elf-g++
AS:=$(PREFIX)/i686-elf-as
CFLAGS?=-O2
CFLAGS=$(CFLAGS?) -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

PROJECT_DIR=.
ISO_DIR=$(PROJECT_DIR)/iso
BIN_DIR=$(PROJECT_DIR)/bin
OBJ_DIR=$(PROJECT_DIR)/obj
KERNEL_DIR=$(PROJECT_DIR)/kernel
ARCHI=i686

CRTI_OBJ=$(OBJ_DIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=$(OBJ_DIR)/crtn.o
BOOT_OBJ=$(OBJ_DIR)/boot.o

OBJS:=kernel.o terminal.o

OBJ_LINK_LIST:= $(BOOT_OBJ) $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(addprefix $(OBJ_DIR)/,$(OBJS)) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:= $(BOOT_OBJ) $(CRTI_OBJ) $(OBJS) $(CRTN_OBJ)



build: iso

myos: $(OBJ_DIR) $(OBJ_LINK_LIST) $(BIN_DIR)
	$(CC) -T $(KERNEL_DIR)/linker.ld -o $(BIN_DIR)/myos.bin $(OBJ_LINK_LIST) $(LFLAGS)

$(OBJ_DIR)/%.o: $(KERNEL_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: $(KERNEL_DIR)/$(ARCHI)/%.s
	$(AS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

iso: myos
	mkdir -p $(ISO_DIR)/isodir/boot/grub
	cp $(BIN_DIR)/myos.bin $(ISO_DIR)/isodir/boot/myos.bin
	cp grub.cfg $(ISO_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_DIR)/myos.iso $(ISO_DIR)/isodir

test: build
	grub-file --is-x86-multiboot $(BIN_DIR)/myos.bin

qemu: build
	qemu-system-i386 -cdrom $(ISO_DIR)/myos.iso

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(ISO_DIR) GPUCache
