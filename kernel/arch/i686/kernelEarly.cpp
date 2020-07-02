#include "kernel/terminal.hpp"
#include "multiboot.hpp"


extern "C" void kernelEarly(std::uint32_t magic, Multiboot* multiboot)
{
    Terminal terminal;
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        terminal.block("Not loaded with multiboot");
    }
}
