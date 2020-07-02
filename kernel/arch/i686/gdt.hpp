#ifndef GDT_HPP
#define GDT_HPP

#include <cstdint>

void initGdt();

struct __attribute__((__packed__)) GdtEntry
{
    std::uint32_t limitLow :16;
    std::uint32_t baseLow :24;
    //attribute byte split into bitfields
    std::uint32_t accessed :1;
    std::uint32_t readWrite :1; //readable for code, writable for data
    std::uint32_t conformingExpandDown :1; //conforming for code, expand down for data
    std::uint32_t executable :1; //1 for code, 0 for data
    std::uint32_t tss :1; //should be 1 for everything but TSS and LDT
    std::uint32_t dpl :2; //priviledge level
    std::uint32_t present :1;
    //and now into granularity
    std::uint32_t limitHigh :4;
    std::uint32_t available :1;
    std::uint32_t always0 :1; //should always be 0
    std::uint32_t big :1; //32bit opcodes for code, uint32_t stack for data
    std::uint32_t gran :1; //1 to use 4k page addressing, 0 for byte addressing
    std::uint32_t baseHigh :8;
};

struct __attribute__((__packed__)) GdtDescriptor
{
    std::uint16_t size;
    GdtEntry* offset;
};

#endif
