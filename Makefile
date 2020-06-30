export HOST=i686-elf
export QEMUARCH=i386
QEMU_MEM?=2G
# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(shell pwd)/sysroot"

export AR=$(HOST)-ar
export AS=$(HOST)-as
export CC=$(HOST)-g++ --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)

export PREFIX=/usr
export EXEC_PREFIX=$(PREFIX)
export BOOTDIR=/boot
export LIBDIR=$(EXEC_PREFIX)/lib
export INCLUDEDIR=$(PREFIX)/include

export CFLAGS=-O2 -g
export CPPFLAGS=

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.

headers:
	mkdir -p "$(SYSROOT)"
	$(MAKE) DESTDIR=$(SYSROOT) -C libcpp install-headers
	$(MAKE) DESTDIR=$(SYSROOT) -C kernel install-headers

build: headers
	$(MAKE) DESTDIR=$(SYSROOT) -C libcpp install
	$(MAKE) DESTDIR=$(SYSROOT) -C kernel install

iso: build
	mkdir -p isodir/boot/grub
	cp sysroot/boot/os.kernel isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o os.iso isodir

qemu: iso
	qemu-system-$(QEMUARCH) -cdrom os.iso -m $(QEMU_MEM)

clean:
	$(MAKE) -C libcpp clean
	$(MAKE) -C kernel clean
	rm -rf sysroot
	rm -rf isodir
	rm -rf os.iso
