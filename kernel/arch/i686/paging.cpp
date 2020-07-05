#include "paging.hpp"
#include <cstddef>

PageDirectoryEntry  pageDirectory[1024]__attribute__ ((aligned(4096)));
PageTableEntry pageTable1[1024] __attribute__ ((aligned(4096)));

void initPaging()
{
    for(std::uint32_t i{0};i<1024;i++)
    {
        pageDirectory[i]={0,1,0,0,0,0,0,0,0,0,0};
        pageTable1[i]={1,1,0,0,0,0,0,0,0,0,i};
    }
    pageDirectory[0].present=1;
    pageDirectory[0].address=reinterpret_cast<std::uint32_t>(pageTable1)>>12;

    asm volatile("mov %0, %%cr3 \n\t"
                 "mov %%cr0, %%eax \n\t"
                 "or %1, %%eax \n\t"
                 "mov %%eax, %%cr0"
                 :
                 : "r" (pageDirectory), "r" (0x80000000)
                 : "%eax"
        );
}
