// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pmm.h>
#include <process.h>

#define P_INIT_EFLAGS 0x202
#define P_INIT_CS     0x8
#define P_EXIT_CODE   0

static int last_pid = 0;
static int ticks_by_priority[5] = {1, 2, 3, 5, 8, 13};

static void start(function_t function, int argc, char *argv[]) {
    function(argc, argv);
    sys_exit(P_EXIT_CODE);
}

static int process_compare(process_t *process, pid_t pid) {
    return process->pid == pid;
}

process_t *new_process(function_t function, int argc, char *argv[]) {
    process_t *process = (process_t *)kmalloc(sizeof(process_t));
    if (process == NULL)
        return NULL;

    process->pid = last_pid++;
    process->status = READY;
    process->children = new_linked_list(process_compare);
    process->exit_status = 0;
    process->priority = ticks_by_priority[5];

    context_t *context =
        (context_t *)((uint64_t)process + K_PROCESS_STACK_SIZE -
                      sizeof(context_t));

    context->rdi = (uint64_t)function;
    context->rsi = (uint64_t)argc;
    context->rdx = (uint64_t)argv;
    context->rip = (uint64_t)&start;
    context->cs = P_INIT_CS;
    context->eflags = P_INIT_EFLAGS;

    context->rsp = (uint64_t)context;
    context->ss = 0x0;

    process->context = context;
    process->channel = NULL;

    process->g_context = get_context_id();

    process->l_child = NULL;
    process->r_child = NULL;
    process->parent = NULL;

    return process;
}
