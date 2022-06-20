#ifndef _IDT_LOADER_H_
#define _IDT_LOADER_H_

#include <defs.h>
#include <interrupts.h>

/**
 * @brief Sets all the Interrupt Descriptor Table entries, 
 * including exception, irq and syscall handlers. 
 * 
 * It also mask out the corresponding bits of master and slave PIC
 */
void load_idt();

#endif /* _IDT_LOADER_H_ */