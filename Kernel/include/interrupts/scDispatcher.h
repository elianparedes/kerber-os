#ifndef _SC_DISPATCHER_H_
#define _SC_DISPATCHER_H_

#include <stdint.h>

// id(%rax) arg0(%rdi) arg1 (%rsi)	arg2 (%rdx)	arg3 (%r10)	arg4 (%r8)	arg5 (%r9)
uint8_t syscall_dispatcher(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t id);

#endif