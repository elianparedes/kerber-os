#include <interrupts/interrupts.h>
#include <interrupts/time.h>

static void (*irq_handlers[])()={
        timer_handler
    };

void set_kbd_handler(void * handler);