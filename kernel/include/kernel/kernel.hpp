#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "kernel/terminal.hpp"

class Kernel
{
public:
    Kernel();
    void main();
private:
    Terminal m_terminal;
};

#endif
