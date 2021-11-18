#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

#include <cstdint>
#include "isr.hpp"

namespace interrupt
{
	void init();
	void triggerInterrupt(std::uint8_t interrupt);
	void setISR(isr::Isr isr, std::uint8_t index);
	void setISR(isr::IsrException isr, std::uint8_t index);
}

#endif
