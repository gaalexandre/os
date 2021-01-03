#ifndef GDT_HPP
#define GDT_HPP

#include <cstdint>
#include <cstddef>

namespace segments
{
    class __attribute__((__packed__)) GdtEntry
    {
    public:
        GdtEntry(const std::uint32_t base, const std::uint32_t limit, const std::uint32_t accessByte, const std::uint32_t flags);
    private:
        std::uint32_t m_limitLow :16;
        std::uint32_t m_baseLow :24;
        //attribute byte split into bitfields
        std::uint32_t m_accessed :1;
        std::uint32_t m_readWrite :1; //readable for code, writable for data
        std::uint32_t m_conformingExpandDown :1; //conforming for code, expand down for data
        std::uint32_t m_executable :1; //1 for code, 0 for data
        std::uint32_t m_tss :1; //should be 1 for everything but TSS and LDT
        std::uint32_t m_dpl :2; //priviledge level
        std::uint32_t m_present :1;
        //and now into granularity
        std::uint32_t m_limitHigh :4;
        std::uint32_t m_available :1;
        std::uint32_t m_always0 :1; //should always be 0
        std::uint32_t m_big :1; //32bit opcodes for code, uint32_t stack for data
        std::uint32_t m_gran :1; //1 to use 4k page addressing, 0 for byte addressing
        std::uint32_t m_baseHigh :8;
    };

    struct __attribute__((__packed__)) Gdt
    {
    public:

    private:
        GdtEntry m_null{0,0,0,0};
        GdtEntry m_code0{0,0xffffffff,0x9a,0b1100};
        GdtEntry m_data0{0,0xffffffff,0x92,0b1100};
        GdtEntry m_code3{0,0xffffffff,0xfa,0b1100};
        GdtEntry m_data3{0,0xffffffff,0xf2,0b1100};
    };

    class __attribute__((__packed__)) Gdtd
    {
    public:
        Gdtd(Gdt* gdt);
    private:
        std::uint16_t m_size;
        Gdt* m_offset;
    };
}


#endif
