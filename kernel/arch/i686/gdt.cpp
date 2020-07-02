#include "gdt.hpp"
#include "cstddef"

constexpr std::size_t NB_ENTRIES{5};
GdtEntry gdt[NB_ENTRIES];

consteval GdtEntry initGdtEntry(const uint32_t base, const uint32_t limit, const uint32_t accessByte, const uint32_t flags)
{
    return GdtEntry{
            limit & 0xffff,
            base & 0xffffff,
            accessByte & 0b1,
            (accessByte >> 1) & 0b1,
            (accessByte >> 2) & 0b1,
            (accessByte >> 3) & 0b1,
            (accessByte >> 4) & 0b1,
            (accessByte >> 5) & 0b11,
            (accessByte >> 7) & 0b1,
            (limit >> 16) & 0xf,
            flags & 0b1,
            (flags >> 1) & 0b1,
            (flags >> 2) & 0b1,
            (flags >> 3) & 0b1,
            (base >> 24) & 0xff
    };
}

void reloadSegments()
{
    __asm__ __volatile__(
        "ljmp $0x08,$1f\n\t"
        "1:\n\t"
        "mov %0, %%ds\n\t"
        "mov %0, %%es\n\t"
        "mov %0, %%fs\n\t"
        "mov %0, %%gs\n\t"
        "mov %0, %%ss;\n\t" : : "r" (0x10)
        );
}

void initGdt()
{
    gdt[0]=initGdtEntry(0,0,0,0);
    gdt[1]=initGdtEntry(0,0xffffffff,0x9a,0b1100);
    gdt[2]=initGdtEntry(0,0xffffffff,0x92,0b1100);
    gdt[3]=initGdtEntry(0,0xffffffff,0xfa,0b1100);
    gdt[4]=initGdtEntry(0,0xffffffff,0xf2,0b1100);

    static GdtDescriptor gdtd{sizeof(gdt),gdt};
    __asm__ __volatile__("lgdt %[gdtd]" : : [gdtd] "m" (gdtd));
    reloadSegments();
}
