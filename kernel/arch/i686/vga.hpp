#ifndef VGA_HPP
#define VGA_HPP

#include <stdint.h>
#include <stddef.h>
#include "higherHalfPaging.hpp"

/* Hardware text mode color constants. */
enum class VgaColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};

static inline uint8_t vgaEntryColor(VgaColor fg, VgaColor bg)
{
    return static_cast<uint8_t>(fg) | static_cast<uint8_t>(bg) << 4;
}

static inline uint16_t vgaEntry(unsigned char uc, uint8_t color)
{
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}


const std::size_t VGA_WIDTH{80};
const std::size_t VGA_HEIGHT{25};
extern std::uint16_t* VGA_MEMORY;

#endif
