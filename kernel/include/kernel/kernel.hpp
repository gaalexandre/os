#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "kernel/terminal.hpp"
#include "kernel/memoryManager.hpp"
#include "kernel/bootInfo.hpp"
#include "kernel/gdtManager.hpp"

class Kernel
{
public:
    Kernel(BootInfo* bootInfo);
    void main();
private:
    GdtManager m_gdtManager;
    MemoryManager m_memoryManager;
    Terminal m_terminal;
};

#endif
