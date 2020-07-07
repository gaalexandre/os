/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

VIRT_BASE = 0xC0000000;

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
        /* Begin putting sections at 1 MiB, a conventional place for kernels to be
           loaded at by the bootloader. */
        . = 1M;
        .boot : ALIGN(4k)
        {
                _multiboot_start = .;
                *(.multiboot.data)
                *(.multiboot.text)
                *(.multiboot.bss)
                *(.multiboot.rodata)
                _multiboot_end = .;
        }

/* First put the multiboot header, as it is required to be put very early
           early in the image or the bootloader won't recognize the file format.
           Next we'll put the .text section. */

        . = . + VIRT_BASE;

        _kernel_start = .;

        .text ALIGN (4K) : AT (ADDR (.text) - VIRT_BASE)
        {
                _text_start = .;
                *(.text)
                _text_end = .;
        }
        /* Read-only data. */
        .rodata ALIGN (4K) : AT (ADDR (.rodata) - VIRT_BASE)
        {
                _rodata_start = .;
                *(.rodata)
                _rodata_end = .;
        }

        /* Read-write data (initialized) */
        .data ALIGN (4K) : AT (ADDR (.data) - VIRT_BASE)
        {
                _data_start = .;
                *(.data)
                _data_end = .;
        }

        /* Read-write data (uninitialized) and stack */
        .bss ALIGN (4K) : AT (ADDR (.bss) - VIRT_BASE)
        {
                _bss_start = .;
                *(COMMON)
                *(.bss)
                *(.bootstrap_stack)
                _bss_end = .;
        }
        /* The compiler may produce other sections, by default it will put them in
           a segment with the same name. Simply add stuff here as needed. */
        _kernel_end = .;
}
