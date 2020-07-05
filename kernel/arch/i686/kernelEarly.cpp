#include "kernel/terminal.hpp"
#include "multiboot.hpp"
#include "gdt.hpp"
#include "paging.hpp"
extern "C" void kernelEarly(std::uint32_t magic, Multiboot* multiboot)
{
    Terminal terminal;
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        terminal.block("Not loaded with multiboot");
    }
    initGdt();
    initPaging();
}
