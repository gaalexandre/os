#include "kernel/isr.hpp"
#include "kernel/terminal.hpp"

struct interrupt::isr::InterruptFrame {

};

void interrupt::isr::stub(InterruptFrame */*frame*/) {
	Terminal t;
	t.printf("interrupt");
	while(true);
}

void interrupt::isr::stubException(InterruptFrame */*frame*/,
								   unsigned int /*errorCode*/) {
	Terminal t;
	t.printf("exception");
	while(true);
}
