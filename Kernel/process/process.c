#include <process/process.h>

static int last_pid = 0;

static void process_start(function_t function){
    function();
    exit_process();
}

process_t * new_process(uint64_t base_addr, function_t function){
    context_t * context = (context_t *)(base_addr + PROCESS_STACK_SIZE - sizeof(context_t));

    context->rdi = function;
    context->rip = &process_start;
	context->cs = 0x8;  // TODO: get cs from gdt 
	context->eflags = 0x202;
	
    process_t * process = base_addr;
    process->context = context;
    process->pid = last_pid++;
    process->terminated = false;
    process->next = NULL;

    return process;
}



