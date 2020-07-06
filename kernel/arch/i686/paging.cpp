#include "paging.hpp"
#include "higherHalfPaging.hpp"
#include <cstddef>

PageDirectoryEntry  pageDirectory[1024] __attribute__ ((aligned(4096)));
PageTableEntry pageTables[1024*256] __attribute__ ((aligned(4096)));

extern const std::uint32_t __attribute__((__section__(".multiboot.rodata"))) pageDirectoryAddress{reinterpret_cast<std::uint32_t>(pageDirectory)};
extern const std::uint32_t __attribute__((__section__(".multiboot.rodata"))) pageTablesAddress{reinterpret_cast<std::uint32_t>(pageTables)};

void correctPaging()
{

}
