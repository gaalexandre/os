export PREFIX="$(HOME)/os-dev/cross"

build: bootstrap kernel bin iso

bootstrap:
	$(PREFIX)/bin/i686-elf-as boot.s -o boot.o

kernel:
	$(PREFIX)/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

bin: bootstrap kernel
	$(PREFIX)/bin/i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

iso: bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

test: build
	grub-file --is-x86-multiboot myos.bin

qemu: build
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -rf isodir *.o *.bin *.iso GPUCache
