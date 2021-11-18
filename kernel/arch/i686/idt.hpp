#ifndef IDT_HPP
#define IDT_HPP

#include <cstddef>
#include <cstdint>
#include "kernel/isr.hpp"

namespace interrupt
{
	class __attribute__((__packed__)) IdtEntry
	{
	public:
		void setOffset(std::uint32_t offset);
		void setFlags(std::int8_t flags);
	private:
		std::uint16_t m_offset1; // offset bits 0..15
		std::uint16_t m_selector{0x08}; // a code segment selector in GDT or LDT
		std::uint8_t m_zero{0};      // unused, set to 0
		std::uint8_t m_flags;         // type and attributes, see below
		std::uint16_t m_offset2; // offset bits 16..31
	};

	class Idt
	{
	public:

		static constexpr std::uint8_t IRQ0 = 0x20;
		static constexpr std::uint8_t IRQ8 = 0x28;

		Idt();
		void setISR(isr::Isr isr, std::uint8_t index);
		void setISR(isr::IsrException isr, std::uint8_t index);
	private:
		void setISR(std::uint32_t isrAddress, std::uint8_t index);
		static constexpr std::uint16_t NB_ENTRIES{256};
		IdtEntry m_entries[NB_ENTRIES];
	};

	class __attribute__((__packed__)) IdtInfo
	{
	public:
		IdtInfo(Idt* idt);
	private:
		std::uint16_t m_size;
		Idt* m_offset;
	};
}

#endif
