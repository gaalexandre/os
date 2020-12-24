#include "kernel/kernel.hpp"
#include <cstdint>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern "C" void kernelMain(const std::uint32_t magic, BootInfo* bootInfo)
{
        if(!checkBootInfo(magic))
        {
                Terminal terminal;
                terminal.block("Invalid boot magic number");
        }
        Kernel kernel{bootInfo};
        kernel.main();
}

Kernel::Kernel(BootInfo* bootInfo) : m_memoryManager{bootInfo}
{
        m_memoryManager.endInit(bootInfo);
}

void Kernel::main()
{
    m_terminal.printf("it seems to work");
}
