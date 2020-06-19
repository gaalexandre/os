#include "terminal.hpp"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t strlen(const char* str)
{
    size_t len{0};
    while (str[len])
        len++;
    return len;
}

void Terminal::initialize()
{
    m_row = 0;
    m_column = 0;
    m_color = vga_entry_color(Vga_color::VGA_COLOR_LIGHT_GREY, Vga_color::VGA_COLOR_BLACK);
    m_buffer = (uint16_t*) 0xB8000;
    for (size_t y{0}; y < VGA_HEIGHT; y++)
    {
        for (size_t x{0}; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            m_buffer[index] = vga_entry(' ', m_color);
        }
    }
}

void Terminal::writestring(const char* data)
{
    write(data, strlen(data));
}

void Terminal::setcolor(uint8_t color)
{
    m_color = color;
}

void Terminal::putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    m_buffer[index] = vga_entry(c, color);
}

void Terminal::putchar(char c)
{
    if(c=='\n')
    {
        newline();
        return;
    }
    putentryat(c, m_color, m_column, m_row);
    if (++m_column == VGA_WIDTH)
        newline();

}

void Terminal::write(const char* data, size_t size)
{
    for (size_t i{0}; i < size; i++)
        putchar(data[i]);
}

void Terminal::newline()
{
    m_column=0;
    if (++m_row == VGA_HEIGHT)
    {
        m_row=0;
    }
}
