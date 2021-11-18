#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <cstdint>

__attribute__((no_caller_saved_registers)) inline void outb(std::uint16_t port, std::uint8_t val)
{
	asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

__attribute__((no_caller_saved_registers)) inline std::uint8_t inb(std::uint16_t port)
{
	std::uint8_t ret;
	asm volatile ( "inb %1, %0"
				   : "=a"(ret)
				   : "Nd"(port) );
	return ret;
}

#endif
