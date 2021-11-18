#include "pic.hpp"
#include "idt.hpp"
#include "assembly.hpp"

constexpr std::uint8_t PIC1{0x20};
constexpr std::uint8_t PIC2{0xA0};
constexpr std::uint8_t PIC1_COMMAND{PIC1};
constexpr std::uint8_t PIC1_DATA{PIC1+1};
constexpr std::uint8_t PIC2_COMMAND{PIC2};
constexpr std::uint8_t PIC2_DATA{PIC2+1};

constexpr std::uint8_t PIC_EOI{0x20};

constexpr std::uint8_t ICW1_ICW4 = 0x01; /* ICW4 (not) needed */
constexpr std::uint8_t ICW1_SINGLE = 0x02; /* Single (cascade) mode */
constexpr std::uint8_t ICW1_INTERVAL4 = 0x04; /* Call address interval 4 (8) */
constexpr std::uint8_t ICW1_LEVEL = 0x08; /* Level triggered (edge) mode */
constexpr std::uint8_t ICW1_INIT = 0x10; /* Initialization - required! */

constexpr std::uint8_t ICW4_8086 = 0x01; /* 8086/88 (MCS-80/85) mode */
constexpr std::uint8_t ICW4_AUTO = 0x02; /* Auto (normal) EOI */
constexpr std::uint8_t ICW4_BUF_SLAVE = 0x08; /* Buffered mode/slave */
constexpr std::uint8_t ICW4_BUF_MASTER = 0x0C; /* Buffered mode/master */
constexpr std::uint8_t ICW4_SFNM = 0x10; /* Special fully nested (not) */

constexpr std::uint8_t OW3_READ_IRR = 0x0a;
constexpr std::uint8_t OW3_READ_ISR = 0x0b;

namespace {
	std::uint8_t _offset1{0}, _offset2{8};
	void init_aux(std::uint8_t offset1, std::uint8_t offset2) {
		outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
		outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

		outb(PIC1_DATA, offset1);
		outb(PIC2_DATA, offset2);

		outb(PIC1_DATA, 4);
		outb(PIC2_DATA, 2);


		outb(PIC1_DATA, ICW4_8086);
		outb(PIC2_DATA, ICW4_8086);
		_offset1 = offset1;
		_offset2 = offset2;
	}

	std::uint16_t irqRegister(std::uint8_t ocw3)
	{
		/* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
		 * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
		outb(PIC1_COMMAND, ocw3);
		outb(PIC2_COMMAND, ocw3);
		return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
	}
}

void interrupt::pic::init(std::uint8_t offset1, std::uint8_t offset2) {
	init_aux(offset1, offset2);

	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
	asm volatile ("sti");
}

void interrupt::pic::remap(std::uint8_t offset1, std::uint8_t offset2) {
	std::uint8_t a1{inb(PIC1_DATA)};
	std::uint8_t a2{inb(PIC2_DATA)};

	init_aux(offset1, offset2);

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void interrupt::pic::disable() {
	init_aux(Idt::IRQ0, Idt::IRQ8);
	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}

void interrupt::pic::setMask(std::uint16_t mask) {
	outb(PIC1_DATA, mask & 0xff);
	outb(PIC2_DATA, mask >> 8);
}

std::uint16_t interrupt::pic::mask() {
	std::uint8_t a1{inb(PIC1_DATA)};
	std::uint8_t a2{inb(PIC2_DATA)};
	return (a2 << 8) | a1;
}

void interrupt::pic::setMaskIrq(IrqLine irqLine) {
	std::uint16_t port;
	std::uint8_t value{static_cast<std::uint8_t>(irqLine)};

	if(value < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		value -= 8;
	}
	value = inb(port) | (1 << value);
	outb(port, value);
}

void interrupt::pic::clearMaskIrq(IrqLine irqLine) {
	std::uint16_t port;
	std::uint8_t value{static_cast<std::uint8_t>(irqLine)};

	if(value < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		value -= 8;
	}
	value = inb(port) & ~(1 << value);
	outb(port, value);
}

void interrupt::pic::sendEoi(IrqLine irqLine) {
	if(static_cast<std::uint8_t>(irqLine) >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}

	outb(PIC1_COMMAND, PIC_EOI);
}

std::uint16_t interrupt::pic::irrRegister() {
	return irqRegister(OW3_READ_IRR);
}

std::uint16_t interrupt::pic::isrRegister() {
	return irqRegister(OW3_READ_ISR);
}

bool interrupt::pic::isSpurious(IrqLine irqLine) {
	std::uint16_t isr = isrRegister();
	return (irqLine == IrqLine::lpt1 && isr & (1 << 7))
		|| (irqLine == IrqLine::secondaryAta && isr & (1 << 15));
}

bool interrupt::pic::isIrq(int interruptNumber) {
	return offset1() <= interruptNumber
		&& offset1() + 16 > interruptNumber;
}

std::uint8_t interrupt::pic::offset1() {
	return _offset1;
}

std::uint8_t interrupt::pic::offset2() {
	return _offset2;
}
