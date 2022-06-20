#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <graphics.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define K_PROCESS_STACK_SIZE 1024 * 4

typedef enum pstatus {
    PAUSED = 0,
    READY = 1
} pstatus_t;

typedef int pid_t;

typedef struct context {
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

    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
} context_t;

typedef struct process process_t;

typedef struct process {
    uint8_t k_stack[K_PROCESS_STACK_SIZE];
    context_t *context;
    pid_t pid;
    pstatus_t status;
    process_t *l_child;
    process_t *r_child;
    process_t *parent;
    context_id_t g_context;
} process_t;

typedef void (*function_t)(char *);

process_t *new_process(function_t function, char *arg);

#endif /* _PROCESS_H_ */