#include <interrupts/interrupts.h>
#include <interrupts/time.h>
#include <drivers/keyboard.h>

static void (*irq_handlers[])()={
        timer_handler, kbd_handler
};
