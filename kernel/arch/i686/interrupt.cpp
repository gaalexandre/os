#include "kernel/interrupt.hpp"
#include "idt.hpp"
#include "pic.hpp"

interrupt::Idt idt;

void interrupt::init()
{
	IdtInfo idtInfo{&idt};

	asm volatile("lidt %[idtInfo]" : : [idtInfo] "m" (idtInfo));
	pic::init(Idt::IRQ0, Idt::IRQ8);
}

void interrupt::triggerInterrupt(std::uint8_t interrupt)
{
  asm volatile("movb %0, point+1\n"
			   "point:\n"
			   "int $0\n"
			   :                /*output*/
			   : "r"(interrupt) /*input*/
			   :                /*clobbered */
	  );
}

void interrupt::setISR(isr::Isr isr, std::uint8_t index) {
	idt.setISR(isr, index);
}

void interrupt::setISR(isr::IsrException isr, std::uint8_t index) {
	idt.setISR(isr, index);
}
