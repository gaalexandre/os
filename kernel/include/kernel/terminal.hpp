#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <stddef.h>
#include <stdint.h>

class Terminal
{

public:
    Terminal();
    int printf(const char* format, ...);
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
