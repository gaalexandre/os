#include "kernel/memoryManager.hpp"

MemoryManager::MemoryManager(BootInfo* bootInfo) : m_virtualMemoryManager{bootInfo}
{

}

void MemoryManager::endInit(BootInfo* bootInfo)
{
    m_virtualMemoryManager.cleanMemory(bootInfo);
}
