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

Terminal::Terminal() :
    m_row{0},
    m_column{0},
    m_color{vgaEntryColor(VgaColor::VGA_COLOR_LIGHT_GREY, VgaColor::VGA_COLOR_BLACK)},
    m_buffer{reinterpret_cast<uint16_t*>(0xB8000)}
{
    for (size_t index{0}; index < VGA_HEIGHT*VGA_WIDTH; index++)
    {
        m_buffer[index] = vgaEntry(' ', m_color);
    }
}

void Terminal::writeString(const char* data)
{
    for (const char* i{data}; *i ; i++)
        putChar(*i);
}

void Terminal::setColor(uint8_t color)
{
    m_color = color;
}

void Terminal::putEntryAt(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    m_buffer[index] = vgaEntry(c, color);
}

void Terminal::putChar(char c)
{
    if(c=='\n')
    {
        newline();
        return;
    }
    putEntryAt(c, m_color, m_column, m_row);
    if (++m_column == VGA_WIDTH)
        newline();

}

void Terminal::write(const char* data, size_t size)
{
    for (size_t i{0}; i < size; i++)
        putChar(data[i]);
}

void Terminal::newline()
{
    m_column=0;
    if (++m_row == VGA_HEIGHT)
    {
        m_row--;
        for(size_t i{VGA_WIDTH};i<VGA_WIDTH*VGA_HEIGHT;i++)
        {
            m_buffer[i-VGA_WIDTH]=m_buffer[i];
        }
    }
}
