#ifndef VIRTUALMEMORYMANAGER_HPP
#define VIRTUALMEMORYMANAGER_HPP

#include "kernel/bootInfo.hpp"

class VirtualMemoryManager
{
public:
    VirtualMemoryManager(BootInfo* bootInfo);
    void cleanMemory(BootInfo* bootInfo);
private:
};

#endif
