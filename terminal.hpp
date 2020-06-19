#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum class Vga_color {
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

static inline uint8_t vga_entry_color(Vga_color fg, Vga_color bg)
{
    return static_cast<int>(fg) | static_cast<int>(bg) << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

class Terminal
{

public:
    Terminal();
    void writestring(const char * data);

private:
    size_t m_row;
    size_t m_column;
    uint8_t m_color;
    uint16_t* m_buffer;

    void setcolor(uint8_t color);
    void putentryat(char c, uint8_t color, size_t x, size_t y);
    void putchar(char c);
    void write(const char* data, size_t size);
    void newline();
};

#endif
