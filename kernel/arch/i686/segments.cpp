#include "kernel/segments.hpp"
#include "gdt.hpp"

segments::Gdt gdt;

void segments::reloadSegments()
{
    asm volatile(
        "ljmp $0x08,$1f\n\t"
        "1:\n\t"
        "mov %0, %%ds\n\t"
        "mov %0, %%es\n\t"
        "mov %0, %%fs\n\t"
        "mov %0, %%gs\n\t"
        "mov %0, %%ss;\n\t" : : "r" (0x10)
        );
}

void segments::init()
{
    Gdtd gdtd{&gdt};

    asm volatile("lgdt %[gdtd]" : : [gdtd] "m" (gdtd));
    reloadSegments();
}
