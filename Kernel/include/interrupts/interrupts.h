/**
 * @date Apr 18, 2010
 * @author anizzomc
 */

#ifndef _INTERRUPS_H_
#define _INTERRUPS_H_

#include <stdint.h>

#define IRQ_01 1

/**
 * @brief Interrupt Request 0 handler
 *
 */
extern void _irq00Handler(void);

/**
 * @brief Interrupt Request 1 handler
 *
 */
extern void _irq01Handler(void);

/**
 * @brief Interrupt Request 2 handler
 *
 */
extern void _irq02Handler(void);

/**
 * @brief Interrupt Request 3 handler
 *
 */
extern void _irq03Handler(void);

/**
 * @brief Interrupt Request 4 handler
 *
 */
extern void _irq04Handler(void);

/**
 * @brief Interrupt Request 5 handler
 *
 */
extern void _irq05Handler(void);

/**
 * @brief Exception 0 handler
 *
 */
extern void _exception0Handler(void);

/**
 * @brief Exception 6 handler
 *
 */
extern void _exception6Handler(void);

/**
 * @brief Saves current CPU state
 * when a Userland process invokes a system call and calls the dispatcher to
 * start the execution of a kernel function.
 *
 * @return uint8_t return value of the syscall executed.
 */
extern uint8_t _syscall_master_handler(void);

/**
 * @brief Clears the IF flag in the FLAGS register and no other flags are
 * affected. Clearing the IF flag causes the processor to ignore maskable
 * external interrupts.
 * @see https://www.felixcloutier.com/x86/cli
 */
extern void _cli(void);

/**
 * @brief Sets the interrupt flag (IF) in the FLAGS register.
 * This allows the processor to respond to maskable hardware interrupts.
 *
 * @see https://www.felixcloutier.com/x86/sti
 */
extern void _sti(void);

/**
 * @brief Stops instruction execution and places the processor in a HALT state.
 *
 * @see https://www.felixcloutier.com/x86/hlt
 */
extern void _hlt(void);

/**
 * @brief
 *
 * @param mask
 */
extern void picMasterMask(uint8_t mask);

/**
 * @brief
 *
 * @param mask
 */
extern void picSlaveMask(uint8_t mask);

/**
 * @brief
 *
 */
extern void haltcpu(void);

extern void _force_timer_int(void);

#endif /* INTERRUPS_H_ */
