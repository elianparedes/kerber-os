#ifndef _PROCESS_
#define _PROCESS_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <process/process.h>

#define PROCESS_STACK_SIZE  4096  //  1 Kib

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
} context_t;

typedef void (*function_t)();

typedef struct process process_t;

typedef struct process {
    uint8_t stack[PROCESS_STACK_SIZE];
    context_t * context;
	int pid;
	bool terminated;
	process_t * next;
} process_t;

process_t * new_process(uint64_t base_addr, function_t function);

#endif