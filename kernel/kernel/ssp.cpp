#include <cstdint>

#if __STDC_HOSTED__
#include <cstdlib>
#elif __is_kernel
#include "kernel/terminal.hpp"
#endif

#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

std::uintptr_t __stack_chk_guard = STACK_CHK_GUARD;


extern "C" __attribute__((noreturn)) void __stack_chk_fail(void)
{
#if __STDC_HOSTED__
    abort();
#elif __is_kernel
    Terminal terminal;
    terminal.printf("Stack smashing detected");
    while(true);
#endif
}
