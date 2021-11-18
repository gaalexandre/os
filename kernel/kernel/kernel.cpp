#include "kernel/kernel.hpp"
#include "kernel/interrupt.hpp"
#include "kernel/segments.hpp"
#include "kernel/bootInfo.hpp"
#include "kernel/virtualMemory.hpp"
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
    (void)bootInfo; // to remove warning (temporary)
    if(!checkBootInfo(magic))
    {
        Terminal terminal;
        terminal.block("Invalid boot magic number");
    }

    segments::init();
    virtualMemory::init();

    Kernel kernel;

    virtualMemory::cleanMemory();

    kernel.main();
}

Kernel::Kernel()
{
	interrupt::init();
}

void Kernel::main()
{
    m_terminal.printf("it seems to work");
}
