#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "kernel/terminal.hpp"
#include "kernel/memoryManager.hpp"
#include "kernel/bootInfo.hpp"

class Kernel
{
public:
    Kernel(BootInfo* bootInfo);
    void main();
private:
    MemoryManager m_memoryManager;
    Terminal m_terminal;
};

#endif
