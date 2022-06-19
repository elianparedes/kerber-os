#ifndef _IRQ_DISPATCHER_H_
#define _IRQ_DISPATCHER_H_

#include <interrupts.h>
#include <time.h>
#include <keyboard.h>

static void (*irq_handlers[])()={
        timer_handler, kbd_handler
};

#endif /* _IRQ_DISPATCHER_H_ */
