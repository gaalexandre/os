#include "kernel/serial.hpp"
#include "assembly.hpp"
#include "pic.hpp"
#include "kernel/interrupt.hpp"
#include <cstddef>
#include <cstdint>
#include <array>

constexpr std::uint32_t COM1 = 0x3f8;
constexpr std::uint32_t COM2 = 0x2f8;
constexpr std::uint32_t COM3 = 0x3e8;
constexpr std::uint32_t COM4 = 0x2e8;

constexpr std::size_t NB_COM_PORT = 4;
constexpr std::size_t QUEUE_BLOCK_SIZE = 14;
constexpr std::size_t QUEUE_SIZE = 10 * QUEUE_BLOCK_SIZE;

class serial::ComPort::Private {
public:
	Private(std::uint32_t comPort) : com{comPort} {}
	const std::uint32_t com;
	std::array<std::uint8_t, QUEUE_SIZE> sendQueue;
	std::array<std::uint8_t, QUEUE_SIZE> readQueue;
	std::size_t readHead = 0;
	std::size_t readTail = 0;
	std::size_t sendHead = 0;
	std::size_t sendTail = 0;
	bool enabled = false;
};

namespace {
	serial::ComPort::Private *_comPorts{nullptr};
	__attribute__((no_caller_saved_registers)) void handleInterrupt(serial::ComPort::Private &comPort) {
		if (!comPort.enabled) {
			return;
		}

		if ((inb(comPort.com + 5) & 0x20)) {
			for (std::size_t i = 0; i < QUEUE_BLOCK_SIZE && comPort.sendHead != comPort.sendTail; i++) {
				outb(comPort.com, comPort.sendQueue[comPort.sendHead]);
				comPort.sendHead++;
				comPort.sendHead %= QUEUE_SIZE;
			}
		}

		while (inb(comPort.com + 5) & 1) {
			comPort.readQueue[comPort.readTail] = inb(comPort.com);
			comPort.readTail++;
			comPort.readTail %= QUEUE_SIZE;
			if (comPort.readHead == comPort.readTail) {
				comPort.readHead++;
				comPort.readHead %= QUEUE_SIZE;
			}
		}
	}

	__attribute__((interrupt)) void com13Interrupt(interrupt::isr::InterruptFrame */*frame*/) {
		handleInterrupt(_comPorts[0]);
		handleInterrupt(_comPorts[2]);
		interrupt::pic::sendEoi(interrupt::pic::IrqLine::serialPort1);
	}

	__attribute__((interrupt)) void com24Interrupt(interrupt::isr::InterruptFrame */*frame*/) {
		handleInterrupt(_comPorts[1]);
		handleInterrupt(_comPorts[3]);
		interrupt::pic::sendEoi(interrupt::pic::IrqLine::serialPort2);
	}
}

std::size_t serial::ComPort::nbComPort() {
	return NB_COM_PORT;
}

serial::ComPort* serial::ComPort::comPort(std::size_t number) {
	static serial::ComPort::Private privates[NB_COM_PORT] {COM1, COM2, COM3, COM4};
	static serial::ComPort comPorts[NB_COM_PORT] {privates[0], privates[1], privates[2], privates[3]};
	if (number > nbComPort()) {
		return nullptr;
	} else {
		return comPorts + number;
	}
}

serial::ComPort::ComPort(Private &priv) :
	m_private{priv}
{
	if (priv.com == COM1) {
		_comPorts = &priv;
	}
}

bool serial::ComPort::enable()  {
	outb(m_private.com + 1, 0x00);
	setBaudDivisor(3);
	outb(m_private.com + 2, 0xC7);
	outb(m_private.com + 4, 0x0B);
	outb(m_private.com + 4, 0x1E);
	outb(m_private.com + 0, 0xAE);

	if(inb(m_private.com + 0) != 0xAE) {
		m_private.enabled = false;
		return false;
	}
	outb(m_private.com + 4, 0x03);

	if (m_private.com & 0x100) {
		interrupt::pic::clearMaskIrq(interrupt::pic::IrqLine::serialPort1);
		interrupt::setISR(com13Interrupt, interrupt::pic::offset1() + static_cast<std::uint8_t>(interrupt::pic::IrqLine::serialPort1));
	} else {
		interrupt::pic::clearMaskIrq(interrupt::pic::IrqLine::serialPort2);
		interrupt::setISR(com24Interrupt, interrupt::pic::offset1() + static_cast<std::uint8_t>(interrupt::pic::IrqLine::serialPort2));
	}
	outb(m_private.com + 1, 0x03);
	m_private.enabled = true;
	return true;
}

void serial::ComPort::disable() {
	outb(m_private.com + 1, 0x00);
	m_private.enabled = false;
}

std::uint8_t serial::ComPort::readWord() const {
	if (m_private.readHead != m_private.readTail) {
		std::size_t index = m_private.readHead;
		m_private.readHead++;
		m_private.readHead %= QUEUE_SIZE;
		return m_private.readQueue[index];
	} else if (inb(m_private.com + 5) & 1) {
		handleInterrupt(m_private);
	}
	return 0;
}

void serial::ComPort::sendWord(std::uint8_t word) const {
	std::size_t newTail = (m_private.sendTail + 1) % QUEUE_SIZE;
	if (m_private.sendHead != newTail) {
		m_private.sendQueue[m_private.sendTail] = word;
		m_private.sendTail = newTail;
		if (inb(m_private.com + 5) & 0x20) {
			handleInterrupt(m_private);
		}
	}
}

void serial::ComPort::setBaudDivisor(std::uint16_t divisor) {
	outb(m_private.com + 3, 0x80);
	outb(m_private.com + 0, divisor & 0xff);
	outb(m_private.com + 1, (divisor >> 8) & 0xff);
	outb(m_private.com + 3, 0x03);
}
