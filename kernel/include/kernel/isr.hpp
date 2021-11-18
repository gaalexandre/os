#ifndef ISR_HPP
#define ISR_HPP

namespace interrupt {
	namespace isr {
		struct InterruptFrame;

		using Isr = void(isr::InterruptFrame*);
		using IsrException = void(isr::InterruptFrame*, unsigned int errorCode);


		__attribute__((interrupt)) void stub(InterruptFrame *frame);
		__attribute__((interrupt)) void stubException(InterruptFrame *frame,
													  unsigned int errorCode);
	}
}
#endif
