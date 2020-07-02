#include "kernel/terminal.hpp"
#include "vga.hpp"
#include <stdarg.h>
static const size_t VGA_WIDTH{80};
static const size_t VGA_HEIGHT{25};
static uint16_t* const VGA_MEMORY{reinterpret_cast<uint16_t*>(0xB8000)};

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int Terminal::printf(const char* format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    while (*format != '\0')
    {
        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;

            write(format, amount);
            format += amount;
            continue;
        }

        const char* format_begun_at = format++;

        if (*format == 'c')
        {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            write(&c, sizeof(c));
        } else if (*format == 'u') {
            format++;
            uint32_t i = va_arg(parameters, uint32_t);
            char str[30];
            int len{0};
            for(len=0;i!=0; i/=10)
            {
                char n=i%10;
                len++;
                str[30-len]='0'+n;
            }
            if(len!=0)
                write(&(str[30-len]), len);
            else
                putChar('0');
        } else if (*format == 'b') {
            format++;
            uint32_t i = va_arg(parameters, uint32_t);
            char str[128];
            int len{0};
            for(len=0;i!=0; i/=2)
            {
                char n=i%2;
                len++;
                str[128-len]='0'+n;
            }
            if(len!=0)
                write(&(str[128-len]), len);
            else
                putChar('0');
        } else if (*format == 'x') {
            format++;
            uint32_t i = va_arg(parameters, uint32_t);
            char str[30];
            int len{0};
            for(len=0;i!=0; i/=16)
            {
                char n=i%16;
                len++;
                str[30-len]=(n<10)?'0'+n:'a'-10+n;
            }
            if(len!=0)
                write(&(str[30-len]), len);
            else
                putChar('0');
        } else if (*format == 'p') {
            format++;
            uint32_t i = va_arg(parameters, uint32_t);
            char str[32];
            int len{0};
            for(len=0;len%8!=0||i!=0; i/=16)
            {
                char n=i%16;
                len++;
                str[32-len]=(n<10)?'0'+n:'a'-10+n;
            }
            if(len!=0)
                write(&(str[32-len]), len);
            else
                putChar('0');
        }
        else if (*format == 'c')
        {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            write(&c, sizeof(c));
        } else if (*format == 'U') {
            format++;
            uint64_t i = va_arg(parameters, uint64_t);
            char str[30];
            int len{0};
            for(len=0;i!=0; i/=10)
            {
                char n=i%10;
                len++;
                str[30-len]='0'+n;
            }
            if(len!=0)
                write(&(str[30-len]), len);
            else
                putChar('0');
        } else if (*format == 'B') {
            format++;
            uint64_t i = va_arg(parameters, uint64_t);
            char str[128];
            int len{0};
            for(len=0;i!=0; i/=2)
            {
                char n=i%2;
                len++;
                str[128-len]='0'+n;
            }
            if(len!=0)
                write(&(str[128-len]), len);
            else
                putChar('0');
        } else if (*format == 'X') {
            format++;
            uint64_t i = va_arg(parameters, uint64_t);
            char str[30];
            int len{0};
            for(len=0;i!=0; i/=16)
            {
                char n=i%16;
                len++;
                str[30-len]=(n<10)?'0'+n:'a'-10+n;
            }
            if(len!=0)
                write(&(str[30-len]), len);
            else
                putChar('0');
        } else if (*format == 'P') {
            format++;
            uint64_t i = va_arg(parameters, uint64_t);
            char str[32];
            int len{0};
            for(len=0;len%8!=0||i!=0; i/=16)
            {
                char n=i%16;
                len++;
                str[32-len]=(n<10)?'0'+n:'a'-10+n;
            }
            if(len!=0)
                write(&(str[32-len]), len);
            else
                putChar('0');
        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            write(str, len);
        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            write(format, len);
            format += len;
        }
    }

    va_end(parameters);
    return 0;
}

void Terminal::block(const char* text)
{
    printf(text);
    while(true);
}

Terminal::Terminal() :
    m_row{0},
    m_column{0},
    m_color{vgaEntryColor(VgaColor::VGA_COLOR_LIGHT_GREY, VgaColor::VGA_COLOR_BLACK)},
    m_buffer{VGA_MEMORY}
{
    for (std::size_t index{0}; index < VGA_HEIGHT*VGA_WIDTH; index++)
    {
        m_buffer[index] = vgaEntry(' ', m_color);
    }
}

void Terminal::setColor(std::uint8_t color)
{
    m_color = color;
}

void Terminal::putEntryAt(char c, std::uint8_t color, std::size_t x, std::size_t y)
{
    const std::size_t index = y * VGA_WIDTH + x;
    m_buffer[index] = vgaEntry(c, color);
}

int Terminal::putChar(char c)
{
    if(c=='\n')
    {
        newline();
        return c;
    }
    putEntryAt(c, m_color, m_column, m_row);
    if (++m_column == VGA_WIDTH)
        newline();
    return c;
}

void Terminal::write(const char* data, std::size_t size)
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
