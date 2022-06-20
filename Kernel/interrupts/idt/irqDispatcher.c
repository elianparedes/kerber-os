#include <irqDispatcher.h>

void irqDispatcher(uint64_t irq) {
    (*irq_handlers[irq])();
}
