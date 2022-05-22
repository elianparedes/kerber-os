#include <interrupts/irqDispatcher.h>

void irqDispatcher(uint64_t irq) {
	(*irq_handlers[irq])();
}

void set_kbd_handler(void * handler){
	picMasterMask(0xFD);
	irq_handlers[IRQ_01]=handler;
}