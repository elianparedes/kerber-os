#include <process/process.h>
#include <process/scheduler.h>

static process_t *current_process = NULL;
static process_t *front_process = NULL;
static process_t *rear_process = NULL;

static uint64_t process_count = 0;

static void enqueue_process(process_t *process){
    if (process_count == 0)
        front_process = process;
    else 
        rear_process->next = process;

    rear_process = process;
    rear_process->next = front_process;

    process_count++;
}

bool add_process(process_t *process){
    if (process_count >= NPROC)
        return false;

    enqueue_process(process);
    return true;
}

void exit_process(){
    //current_process->terminated = true;
    //_force_schedule();
    //...
}

process_t * get_current_process(){
    return current_process;
}

uint64_t * schedule(uint64_t * rsp){
    if (process_count == 0)
        return rsp;    

    if (current_process != NULL){
        current_process->context = rsp;
        current_process = current_process->next;
    } else 
        current_process = front_process;
    
    return current_process->context;
}
        