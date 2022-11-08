#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <graphics.h>
#include <lib/dataDescriptor.h>
#include <linked_list.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LOWEST               1
#define MEDIUM               5
#define HIGHEST              10

#define K_PROCESS_STACK_SIZE (1024 * 4)

typedef enum pstatus {
    WAITING = 0,
    READY,
    TERMINATED,
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
    process_t *parent;
    context_id_t g_context;
    dataDescriptor_t dataDescriptors[128];
    size_t dataD_index;
    void *channel;
    list_ptr children;
    int exit_status;
    int priority;
    char **argv;
    int argc;
} process_t;

typedef int (*function_t)(int, char *[]);

/**
 * @brief Creates a new process.
 *
 * @param main main function of the process
 * @param argc argument count that the main function receives
 * @param argv an array containing all the arguments
 * @return process_t* pointer to the created process, or NULL if the process
 * could not be created
 */
process_t *new_process(function_t main, int argc, char *argv[]);

void free_process(process_t *process);

#endif /* _PROCESS_H_ */