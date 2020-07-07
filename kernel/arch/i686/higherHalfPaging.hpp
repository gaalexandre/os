#ifndef HIGHERHALFPAGING_HPP
#define HIGHERHALFPAGING_HPP

#include <cstdint>

constexpr std::uint32_t VIRT_BASE{0xC0000000};

extern std::uint32_t _kernel_start
    , _kernel_end
    , _multiboot_start
    , _multiboot_end
    , _text_start
    , _text_end
    , _rodata_start
    , _rodata_end
    , _data_start
    , _data_end
    , _bss_start
    , _bss_end;

#endif
