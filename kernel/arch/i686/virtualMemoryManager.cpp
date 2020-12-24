#include "kernel/virtualMemoryManager.hpp"

#include <cstddef>
#include <cstdint>

#include "virtualMemory.hpp"
#include "higherHalfPaging.hpp"
#include "vga.hpp"
#include "kernel/terminal.hpp"

PageDirectoryEntry  pageDirectory[1024] __attribute__ ((aligned(4096)));
PageTableEntry pageTables[1024*256] __attribute__ ((aligned(4096)));

extern const std::uint32_t __attribute__((__section__(".multiboot.rodata"))) pageDirectoryAddress{reinterpret_cast<std::uint32_t>(pageDirectory)};
extern const std::uint32_t __attribute__((__section__(".multiboot.rodata"))) pageTablesAddress{reinterpret_cast<std::uint32_t>(pageTables)};

constexpr std::uint32_t PAGE_LENGTH{4096};
constexpr std::uint32_t PAGE_TABLE_LENGTH{PAGE_LENGTH*1024};

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


void clearTLB()
{
    asm inline(
        "mov %%cr3, %%eax\n\t"
        "mov %%eax, %%cr3"
        :
        :
        :"%eax");
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
        pageTables[j].readWrite=0;
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_rodata_start);i+=PAGE_LENGTH)
    {
        terminal.block("_rodata_start");
        j++;
    }

    for(;i<reinterpret_cast<std::uint32_t>(&_rodata_end);i+=PAGE_LENGTH)
    {
        pageTables[j].readWrite=0;
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
    VGA_MEMORY=reinterpret_cast<std::uint16_t*>(((j/1024+(VIRT_BASE>>22))<<22)+((j%1024)<<12)+(reinterpret_cast<std::uint32_t>(VGA_MEMORY)&0x00000fff));
    for(std::uint32_t kernelEnd{i};i < kernelEnd+VGA_WIDTH*VGA_HEIGHT*sizeof(*VGA_MEMORY);i+=PAGE_LENGTH)
    {
        pageTables[j]=pageTables[(reinterpret_cast<std::uint32_t>(VGA_MEMORY_ADR)>>12)&0x3ff];
        j++;
    }
    for(std::size_t k{1};k-1<j/1024;k++)
    {
        pageDirectory[k+(VIRT_BASE>>22)]={1,1,0,0,0,0,0,0,0,0,reinterpret_cast<std::uint32_t>(&(pageTables[k*1024]))};
    }

    clearTLB();
}

void VirtualMemoryManager::cleanMemory(BootInfo* bootInfo)
{
    std::uint32_t j{0};
    for(std::uint32_t i{VIRT_BASE};i<reinterpret_cast<std::uint32_t>(&_kernel_start);i+=PAGE_LENGTH)
    {
        pageTables[j].present=0;
        j++;
    }
    clearTLB();
}
