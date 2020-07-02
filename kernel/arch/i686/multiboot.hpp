#ifndef MULTIBOOT_HPP
#define MULTIBOOT_HPP

#include <cstdint>

constexpr std::uint32_t MULTIBOOT_BOOTLOADER_MAGIC{0x2BADB002};

struct Multiboot
{
    std::uint32_t flags;
    std::uint32_t mem_lower;
    std::uint32_t mem_upper;
    std::uint32_t boot_device;
    std::uint32_t cmdline;
    std::uint32_t mods_count;
    std::uint32_t mods_addr;
    std::uint32_t syms[4];
    std::uint32_t mmap_length;
    std::uint32_t mmap_addr;
    std::uint32_t drives_length;
    std::uint32_t drives_addr;
    std::uint32_t config_table;
    std::uint32_t boot_loader_name;
    std::uint32_t apm_table;
    std::uint32_t vbe_control_info;
    std::uint32_t vbe_mode_info;
    std::uint16_t vbe_mode;
    std::uint16_t vbe_interface_seg;
    std::uint16_t vbe_interface_off;
    std::uint16_t vbe_interface_len;
    std::uint64_t framebuffer_addr;
    std::uint32_t framebuffer_pitch;
    std::uint32_t framebuffer_width;
    std::uint32_t framebuffer_height;
    std::uint8_t framebuffer_bpp;
    std::uint8_t framebuffer_type;
    std::uint8_t color_info[6];
};

struct Mmap
{
    std::uint32_t size;
    std::uint64_t base_addr;
    std::uint64_t lenght;
    std::uint32_t type;
};

#endif
