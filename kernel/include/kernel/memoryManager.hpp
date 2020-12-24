#ifndef MEMORYMANAGER_HPP
#define MEMORYMANAGER_HPP

#include <cstddef>
#include "kernel/virtualMemoryManager.hpp"
#include "kernel/bootInfo.hpp"


class MemoryManager
{
public:
    MemoryManager(BootInfo* bootInfo);
    void endInit(BootInfo* bootInfo);
private:
    VirtualMemoryManager m_virtualMemoryManager;
};

#endif
