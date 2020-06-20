#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum class VgaColor {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vgaEntryColor(VgaColor fg, VgaColor bg)
{
    return static_cast<int>(fg) | static_cast<int>(bg) << 4;
}

static inline uint16_t vgaEntry(unsigned char uc, uint8_t color)
{
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

class Terminal
{

public:
    Terminal();
    void writeString(const char * data);
    void writeAddress(const void * address);
private:
    size_t m_row;
    size_t m_column;
    uint8_t m_color;
    uint16_t* m_buffer;

    void setColor(uint8_t color);
    void putEntryAt(char c, uint8_t color, size_t x, size_t y);
    void putChar(char c);
    void write(const char* data, size_t size);
    void newline();
};

#endif
