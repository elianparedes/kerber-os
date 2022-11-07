// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pmm.h>
#include <process.h>

#define P_INIT_EFLAGS 0x202
#define P_INIT_CS     0x8
#define P_SS          0
#define P_EXIT_CODE   0

static int last_pid = 0;
static int ticks_by_priority[6] = {1, 2, 3, 5, 8, 13};

static void start(function_t function, int argc, char *argv[]) {
    int status = function(argc, argv);
    sys_exit(status);
}

static int search_by_pid(process_t *process, pid_t pid) {
    return process->pid == pid;
}

static char **get_argv_copy(int argc, char *argv[]) {
    if (argc == 0 || argv == NULL)
        return NULL;

    char **argv_copy = kmalloc(argc * sizeof(char *));
    for (size_t i = 0; i < argc; i++) {
        argv_copy[i] = kmalloc(strlen(argv[i]) + 1);
        strcpy(argv_copy[i], argv[i]);
    }

    return argv_copy;
}

static context_t *get_init_context(process_t *process, function_t main,
                                   int argc, char *argv[]) {
    context_t *context =
        (context_t *)((uint64_t)process + K_PROCESS_STACK_SIZE -
                      sizeof(context_t));

    context->rdi = (uint64_t)main;
    context->rsi = (uint64_t)argc;
    context->rdx = (uint64_t)argv;
    context->rip = (uint64_t)&start;

    context->cs = P_INIT_CS;
    context->eflags = P_INIT_EFLAGS;
    context->ss = P_SS;

    context->rsp = (uint64_t)context;

    return context;
}

process_t *new_process(function_t main, int argc, char *argv[]) {
    process_t *process = (process_t *)kmalloc(sizeof(process_t));
    if (process == NULL)
        return NULL;

    process->argv = get_argv_copy(argc, argv);
    process->argc = argc;

    process->pid = last_pid++;
    process->priority = 10;

    process->status = READY;
    process->exit_status = -1;

    process->channel = NULL;
    process->parent = NULL;
    process->children = new_linked_list(search_by_pid);

    process->context =
        get_init_context(process, main, process->argc, process->argv);
    process->g_context = get_context_id();

    /* Creates stdin in dataDescriptor 0*/
    process->dataDescriptors[0] = create_dataDescriptor(STD_T, READ_MODE);

    /* Creates stdout in dataDescriptor 0*/
    process->dataDescriptors[1] = create_dataDescriptor(STD_T, WRITE_MODE);

    process->dataD_index = 2;

    return process;
}

void free_process(process_t *process) {
    free_list(process->children);

    // free arguments
    if (process->argc) {
        for (size_t i = 0; i < process->argc; i++)
            kfree(process->argv[i]);

        kfree(process->argv);
    }

    kfree(process);
}
