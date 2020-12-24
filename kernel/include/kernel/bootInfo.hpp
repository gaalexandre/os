#ifndef BOOTINFO_HPP
#define BOOTINFO_HPP

#include <cstdint>

struct BootInfo;

bool checkBootInfo(std::uint32_t magic);

#endif
