#ifndef PIC_HPP
#define PIC_HPP

#include <cstdint>

namespace interrupt
{
	namespace pic
	{
		void init(std::uint8_t offset1, std::uint8_t offset2);
		void remap(std::uint8_t offset1, std::uint8_t offset2);
		void disable();
		void setMask(std::uint16_t mask);
		std::uint16_t mask();

		enum class IrqLine : std::uint8_t
		{
			timer = 0,
			keyboard,
			slave,
			serialPort2,
			serialPort1,
			lpt2,
			floppyDisk,
			lpt1,
			cmos,
			free1,
			free2,
			free3,
			ps2Mouse,
			fpu,
			primaryAta,
			secondaryAta
		};
		void setMaskIrq(IrqLine irqLine);
		void clearMaskIrq(IrqLine irqLine);
		__attribute__((no_caller_saved_registers)) void sendEoi(IrqLine irqLine);
		std::uint16_t irrRegister();
		std::uint16_t isrRegister();
		bool isSpurious(IrqLine irqLine);
		bool isIrq(int interruptNumber);
		std::uint8_t offset1();
		std::uint8_t offset2();
	}
}

#endif
