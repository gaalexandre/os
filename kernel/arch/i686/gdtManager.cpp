#include "kernel/gdtManager.hpp"
#include "gdt.hpp"

consteval GdtEntry initGdtEntry(const std::uint32_t base, const std::uint32_t limit, const std::uint32_t accessByte, const std::uint32_t flags)
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

void GdtManager::reloadSegments()
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

GdtManager::GdtManager() :
    m_gdt{{
    initGdtEntry(0,0,0,0),
    initGdtEntry(0,0xffffffff,0x9a,0b1100),
    initGdtEntry(0,0xffffffff,0x92,0b1100),
    initGdtEntry(0,0xffffffff,0xfa,0b1100),
    initGdtEntry(0,0xffffffff,0xf2,0b1100)
    }},
    m_gdtd{sizeof(m_gdt.entries),m_gdt.entries}
{
    asm volatile("lgdt %[gdtd]" : : [gdtd] "m" (m_gdtd));
    reloadSegments();
}
