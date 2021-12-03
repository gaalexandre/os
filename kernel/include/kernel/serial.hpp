#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <cstddef>
#include <cstdint>

namespace serial {

	class ComPort
	{
	public:
		static std::size_t nbComPort();
		static ComPort* comPort(std::size_t number);

		bool enable();
		void disable();
		std::uint8_t readWord() const;
		void sendWord(std::uint8_t word) const;
		void setBaudDivisor(std::uint16_t divisor);

		class Private;
	private:
		Private &m_private;
		ComPort(Private &priv);
	};
}

#endif
