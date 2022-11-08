#ifndef _SC_DISPATCHER_H_
#define _SC_DISPATCHER_H_

#include <stdint.h>

/**
 * @brief Manages all syscalls.
 *
 * @param arg0 rdi 64-bit CPU register
 * @param arg1 rsi 64-bit CPU register
 * @param arg2 rdx 64-bit CPU register
 * @param arg3 r10 64-bit CPU register
 * @param arg4 r8 64-bit CPU register
 * @param arg5 r9 64-bit CPU register
 * @param id rax 64-bit CPU register
 * @return uint8_t the return value of the syscall that was executed
 */
int syscall_dispatcher(uint64_t arg0, uint64_t arg1, uint64_t arg2,
                       uint64_t arg3, uint64_t arg4, uint64_t arg5,
                       uint64_t id);

#endif