// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <irqDispatcher.h>

void irqDispatcher(uint64_t irq) {
    (*irq_handlers[irq])();
}
