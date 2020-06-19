export PREFIX="$(HOME)/os-dev/cross"

build: bootstrap kernel bin iso

bootstrap:
	$(PREFIX)/bin/i686-elf-as boot.s -o boot.o

kernel:
	$(PREFIX)/bin/i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	$(PREFIX)/bin/i686-elf-g++ -c terminal.cpp -o terminal.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

bin: bootstrap kernel
	$(PREFIX)/bin/i686-elf-g++ -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o terminal.o -lgcc

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
