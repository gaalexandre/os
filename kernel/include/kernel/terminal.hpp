#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <cstddef>
#include <cstdint>

class Terminal
{

public:
    Terminal();
    int printf(const char* format, ...);
    int putChar(char c);
private:
    std::size_t m_row;
    std::size_t m_column;
    std::uint8_t m_color;
    std::uint16_t* m_buffer;

    void setColor(std::uint8_t color);
    void putEntryAt(char c, std::uint8_t color, std::size_t x, std::size_t y);
    void write(const char* data, std::size_t size);
    void newline();
};

#endif
