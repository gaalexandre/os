#ifndef PAGING_HPP
#define PAGING_HPP

#include <cstdint>

void correctPaging();

struct __attribute__((__packed__)) PageDirectoryEntry
{
    std::uint32_t present:1;
    std::uint32_t readWrite:1;
    std::uint32_t user:1;
    std::uint32_t writeThrough:1;
    std::uint32_t cacheDisable:1;
    std::uint32_t accessed:1;
    std::uint32_t dirty:1; //for 4MB page 0 if not
    std::uint32_t size:1;
    std::uint32_t global:1;
    std::uint32_t ignored:3; //put what you want
    std::uint32_t address:20;
};

struct __attribute__((__packed__)) PageTableEntry
{
    std::uint32_t present:1;
    std::uint32_t readWrite:1;
    std::uint32_t user:1;
    std::uint32_t writeThrough:1;
    std::uint32_t cacheDisable:1;
    std::uint32_t accessed:1;
    std::uint32_t dirty:1;
    std::uint32_t memoryType:1; //only if PAT is enabled
    std::uint32_t global:1;
    std::uint32_t ignored:3; //put what you want
    std::uint32_t address:20;
};


#endif
