#ifndef _REGISTERS_H_
#define _REGISTERS_H_
#include <stdint.h>

typedef enum {
    EXCEPTION = 0,
    KBD
} request_t;

typedef struct cpu_state {
    uint64_t rsp;
    uint64_t rflags;
    uint64_t rip;

    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;

    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

} cpu_state_t;

void save_cpu_state(cpu_state_t *cpu, request_t request);
cpu_state_t *get_cpu_state();
int copy_cpu_state(cpu_state_t *cpu_ptr, request_t request);
#endif