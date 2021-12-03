#include "cpuid.hpp"
#include <cpuid.h>
#include <cstdint>

bool cpuid::hasAPIC()
{
    constexpr unsigned int EDX_APIC{1<<9};
    unsigned int unused, edx;
    if(__get_cpuid(1, &unused, &unused, &unused, &edx))
        return edx & EDX_APIC;
    else
        return false;
}
