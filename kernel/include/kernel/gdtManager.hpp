#ifndef GDTMANAGER_HPP
#define GDTMANAGER_HPP

#include "arch/gdt.hpp"

struct GdtManager
{
public:
    GdtManager();
    void reloadSegments();
private:
    Gdt m_gdt;
    GdtDescriptor m_gdtd;
};

#endif
