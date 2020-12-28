#include "kernel/virtualMemoryManager.hpp"

#include <cstddef>
#include <cstdint>

#include "virtualMemory.hpp"
#include "higherHalfPaging.hpp"
#include "vga.hpp"
#include "kernel/terminal.hpp"

PageDirectoryEntry  pageDirectory[NB_ENTRIES] __attribute__ ((aligned(4096)));
PageTableEntry kernelPageTables[NB_ENTRIES * KERNEL_PAGE_TABLE_SIZE] __attribute__ ((aligned(4096)));

const std::uint32_t __attribute__((__section__(".multiboot.rodata"))) pageDirectoryAddress{reinterpret_cast<std::uint32_t>(pageDirectory)-VIRT_BASE};
const std::uint32_t __attribute__((__section__(".multiboot.rodata"))) pageTablesAddress{reinterpret_cast<std::uint32_t>(kernelPageTables)-VIRT_BASE};

extern "C" __attribute__((__section__(".multiboot.text"))) void initHigherHalfPaging()
{
    if((reinterpret_cast<std::uint32_t>(&_kernel_end)-VIRT_BASE+PAGE_LENGTH*2)/PAGE_TABLE_LENGTH>KERNEL_PAGE_TABLE_SIZE)
    {
        std::uint16_t *vga=reinterpret_cast<std::uint16_t*>(0xB8000);
        std::uint16_t color=static_cast<uint8_t>(7) | static_cast<uint8_t>(1) << 4;
        for (std::size_t index{0}; index < VGA_HEIGHT*VGA_WIDTH; index++)
        {
            vga[index] = static_cast<uint16_t>(' ') | color << 8;
        }
        char error[]{"kernel too big increase KERNEL_PAGE_TABLE_SIZE"};
        for(std::size_t i{0};error[i];i++)
            vga[i] = static_cast<uint16_t>(error[i]) | color << 8;
        while(true);
    }

    std::uint32_t* pageDirectoryPtr=reinterpret_cast<std::uint32_t*>(pageDirectoryAddress);
    std::uint32_t* pageTablesPtr=reinterpret_cast<std::uint32_t*>(pageTablesAddress);
    for(std::uint32_t i{0};i<NB_ENTRIES;i++)
    {
        pageDirectoryPtr[i]=0x2;
    }
    for(std::uint32_t j{0};j*PAGE_TABLE_LENGTH<reinterpret_cast<std::uint32_t>(&_kernel_end)-VIRT_BASE;j++)
    {
        pageDirectoryPtr[j]|=0x1|reinterpret_cast<std::uint32_t>(&(pageTablesPtr[j*NB_ENTRIES]));
        pageDirectoryPtr[j+(VIRT_BASE>>22)]|=0x1|reinterpret_cast<std::uint32_t>(&(pageTablesPtr[j*NB_ENTRIES]));
        for(std::uint32_t i{0};i<NB_ENTRIES;i++)
        {
            std::uint32_t index{i+j*NB_ENTRIES};
            if(index*4096+VIRT_BASE>=reinterpret_cast<std::uint32_t>(&_kernel_end))
                pageTablesPtr[index]=0;
            else
                pageTablesPtr[index]=0x3|(index*PAGE_LENGTH);
        }
    }

    pageDirectoryPtr[NB_ENTRIES-1]=0x3|pageDirectoryAddress;

    asm volatile("mov %0, %%cr3 \n\t"
                 :
                 : "r" (pageDirectoryPtr)
        );
}


void clearTLB()
{
    asm inline(
        "mov %%cr3, %%eax\n\t"
        "mov %%eax, %%cr3"
        :
        :
        :"%eax");
}

inline void* getVirtualAddress(std::uint32_t pageDirectoryIndex,std::uint32_t pageTableIndex, void* addressInPage)
{
    return reinterpret_cast<void*>((pageDirectoryIndex <<22) | ((pageTableIndex <<12) & 0x3ff000) | (reinterpret_cast<std::uint32_t>(addressInPage) & 0xfff));
}

VirtualMemoryManager::VirtualMemoryManager(BootInfo* bootInfo)
{
    Terminal terminal;

    for(PageDirectoryEntry* i{pageDirectory};i->present;i++)
    {
        i->present=0;
    }

    std::uint32_t i{VIRT_BASE};
    std::uint32_t j{0};

    for(;i<reinterpret_cast<std::uint32_t>(&_kernel_start);i+=PAGE_LENGTH)
    {
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_text_start);i+=PAGE_LENGTH)
    {
        terminal.block("_text_start");
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_text_end);i+=PAGE_LENGTH)
    {
        kernelPageTables[j].readWrite=0;
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_rodata_start);i+=PAGE_LENGTH)
    {
        terminal.block("_rodata_start");
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_rodata_end);i+=PAGE_LENGTH)
    {
        kernelPageTables[j].readWrite=0;
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_data_start);i+=PAGE_LENGTH)
    {
        terminal.block("_data_start");
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_data_end);i+=PAGE_LENGTH)
    {

        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_bss_start);i+=PAGE_LENGTH)
    {
        terminal.block("_bss_start");
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_bss_end);i+=PAGE_LENGTH)
    {
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_kernel_end);i+=PAGE_LENGTH)
    {
        terminal.block("_kernel_start");
        j++;
    }

    std::uint16_t* VGA_MEMORY_ADR{VGA_MEMORY};
    VGA_MEMORY=reinterpret_cast<std::uint16_t*>(getVirtualAddress(j/NB_ENTRIES+(VIRT_BASE>>22), j, VGA_MEMORY));
    for(std::uint32_t kernelEnd{i};i < kernelEnd+VGA_WIDTH*VGA_HEIGHT*sizeof(*VGA_MEMORY);i+=PAGE_LENGTH)
    {
        kernelPageTables[j]=kernelPageTables[((reinterpret_cast<std::uint32_t>(VGA_MEMORY_ADR)>>12)&0x3ff)+(i-kernelEnd)/PAGE_LENGTH];
        j++;
    }

    for(std::size_t k{1}; k < 1 + j/NB_ENTRIES;k++)
    {
        pageDirectory[k+(VIRT_BASE>>22)]={1,1,0,0,0,0,0,0,0,0,(pageTablesAddress+(k*NB_ENTRIES*sizeof(PageTableEntry)))>>12};
    }

    clearTLB();
}

void VirtualMemoryManager::cleanMemory(BootInfo* bootInfo)
{

    // free the lower half memory since all of it is in the higher half
    for(PageDirectoryEntry* i{pageDirectory};i->present;i++)
    {
        i->present=0;
    }

    // free the memory before the kernel starting from VIRT_BASE
    std::uint32_t j{0};
    for(std::uint32_t i{VIRT_BASE};i<reinterpret_cast<std::uint32_t>(&_kernel_start);i+=PAGE_LENGTH)
    {
        kernelPageTables[j].present=0;
        j++;
    }
    clearTLB();
}
