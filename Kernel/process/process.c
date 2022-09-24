#include <pmm.h>
#include <process.h>

#define P_INIT_EFLAGS 0x202
#define P_INIT_CS     0x8
#define P_EXIT_CODE   0

static int last_pid = 0;

static void process_start(function_t function, char *arg) {
    function(arg);
    sys_exit(P_EXIT_CODE);
}

process_t *new_process(function_t function, char *arg) {
    process_t *process = (process_t *)kmalloc(sizeof(process_t));
    if (process == NULL)
        return NULL;

    process->pid = last_pid++;
    process->status = READY;

    context_t *context =
        (context_t *)((uint64_t)process + K_PROCESS_STACK_SIZE -
                      sizeof(context_t));

    context->rsi = (uint64_t)arg;
    context->rdi = (uint64_t)function;
    context->rip = (uint64_t)&process_start;
    context->cs = P_INIT_CS;
    context->eflags = P_INIT_EFLAGS;

    context->rsp = (uint64_t)context;
    context->ss = 0x0;

    process->context = context;

    process->g_context = get_context_id();

    process->l_child = NULL;
    process->r_child = NULL;
    process->parent = NULL;

    return process;
}
