#ifndef _IRQ_DISPATCHER_H_
#define _IRQ_DISPATCHER_H_

#include <interrupts.h>
#include <keyboard.h>
#include <time.h>

static void (*irq_handlers[])() = {timer_handler, kbd_handler};

#endif /* _IRQ_DISPATCHER_H_ */
