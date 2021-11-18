#include "idt.hpp"
#include <cstdint>

namespace {
	bool isTrap(std::uint8_t index) {
		return
			index == 1 ||
			index == 3 ||
			index == 4;
	}

	bool hasErrorCode (std::uint8_t index) {
		return
			index == 8 ||
			index == 10 ||
			index == 11 ||
			index == 12 ||
			index == 13 ||
			index == 14 ||
			index == 17 ||
			index == 30;
	}
}

interrupt::IdtInfo::IdtInfo(Idt* idt):
	m_size{sizeof(Idt)},
	m_offset{idt}
{

}

void interrupt::IdtEntry::setOffset(std::uint32_t offset) {
	m_offset1 = offset & 0xffff;
	m_offset2 = (offset >> 16) & 0xffff;
}

void interrupt::IdtEntry::setFlags(std::int8_t flags) {
	m_flags = flags;
}

interrupt::Idt::Idt() {
	for (int i = 0; i < 256; i++) {
		setISR(isr::stub, i);
		setISR(isr::stubException, i);
	}
}

void interrupt::Idt::setISR(isr::IsrException isr, std::uint8_t index) {
	if (hasErrorCode(index))
		setISR(reinterpret_cast<std::uint32_t>(isr), index);
}

void interrupt::Idt::setISR(isr::Isr isr, std::uint8_t index) {
	if (!hasErrorCode(index))
		setISR(reinterpret_cast<std::uint32_t>(isr), index);
}

void interrupt::Idt::setISR(std::uint32_t isrAdress, std::uint8_t index) {
	m_entries[index].setOffset(isrAdress);
	if (isTrap(index)) {
		m_entries[index].setFlags(0x8f);
	} else {
		m_entries[index].setFlags(0x8e);
	}
}
