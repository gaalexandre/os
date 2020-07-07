#include "higherHalfPaging.hpp"


extern const std::uint32_t pageDirectoryAddress, pageTablesAddress;

extern "C" __attribute__((__section__(".multiboot.text"))) void initHigherHalfPaging()
{
    std::uint32_t* pageDirectoryPtr=reinterpret_cast<std::uint32_t*>(pageDirectoryAddress-VIRT_BASE);
    std::uint32_t* pageTablesPtr=reinterpret_cast<std::uint32_t*>(pageTablesAddress-VIRT_BASE);
    for(std::uint32_t i{0};i<1024;i++)
    {
        pageDirectoryPtr[i]=0x2;
    }
    for(std::uint32_t j{0};j*0x400000<reinterpret_cast<std::uint32_t>(&_kernel_end)-VIRT_BASE;j++)
    {
        pageDirectoryPtr[j]|=0x1|reinterpret_cast<std::uint32_t>(&(pageTablesPtr[j*1024]));
        pageDirectoryPtr[j+(VIRT_BASE>>22)]|=0x1|reinterpret_cast<std::uint32_t>(&(pageTablesPtr[j*1024]));
        for(std::uint32_t i{0};i<1024;i++)
        {
            std::uint32_t index{i+j*1024};
            if(index*4096+VIRT_BASE>=reinterpret_cast<std::uint32_t>(&_kernel_end))
                pageTablesPtr[index]=0;
            else
                pageTablesPtr[index]=0x3|(index*0x1000);
        }
    }

    asm volatile("mov %0, %%cr3 \n\t"
                 :
                 : "r" (pageDirectoryPtr)
        );
}
