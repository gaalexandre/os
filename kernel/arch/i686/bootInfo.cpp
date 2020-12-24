#include "multiboot.hpp"

bool checkBootInfo(std::uint32_t magic)
{
    return magic == MULTIBOOT_BOOTLOADER_MAGIC;
}
