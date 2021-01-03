#include "gdt.hpp"

segments::GdtEntry::GdtEntry(const std::uint32_t base, const std::uint32_t limit, const std::uint32_t accessByte, const std::uint32_t flags) :
    m_limitLow{limit & 0xffff},
    m_baseLow{base & 0xffffff},
    m_accessed{accessByte & 0b1},
    m_readWrite{(accessByte >> 1) & 0b1},
    m_conformingExpandDown{(accessByte >> 2) & 0b1},
    m_executable{(accessByte >> 3) & 0b1},
    m_tss{(accessByte >> 4) & 0b1},
    m_dpl{(accessByte >> 5) & 0b11},
    m_present{(accessByte >> 7) & 0b1},
    m_limitHigh{(limit >> 16) & 0xf},
    m_available{flags & 0b1},
    m_always0{(flags >> 1) & 0b1},
    m_big{(flags >> 2) & 0b1},
    m_gran{(flags >> 3) & 0b1},
    m_baseHigh{(base >> 24) & 0xff}
{

}

segments::Gdtd::Gdtd(segments::Gdt* gdt) : m_size{sizeof(segments::Gdt)}, m_offset{gdt}
{

}
