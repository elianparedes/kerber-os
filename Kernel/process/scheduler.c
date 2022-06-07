#include <process/scheduler.h>
#include <mem/pmm.h>

typedef struct node node_t;

typedef struct node {
    process_t * process;
    node_t * next;
} node_t;

static node_t *current_node = NULL;
static node_t *front_node = NULL;
static node_t *rear_node = NULL;

static uint64_t process_count = 0;

extern void _force_schedule();

static bool enqueue_process(process_t *process){

    node_t * node = kmalloc(sizeof(node_t));
    if (node == NULL)
        return false;

    node->process = process;
    if (process_count == 0)
        front_node = node;
    else 
        rear_node->next = node;

    rear_node = node;
    rear_node->next = front_node;

    process_count++;
    return true;
}

bool add_process(function_t main){
    if (process_count >= MAX_PROC_COUNT)
        return false;
    
    process_t *process = new_process(main);
    if (process == NULL)
        return false;

    return enqueue_process(process);
}

void exit_process(){
    current_node->process->terminated = true;
    _force_schedule();
}

process_t * get_current_process(){
    return current_node->process;
}

uint64_t * schedule(uint64_t * rsp){
    if (process_count == 0)
        return rsp;    

    if (current_node != NULL){
        current_node->process->context = rsp;

        // skip terminated processes to clean them up later
        do 
            current_node = current_node->next;
        while(current_node->process->terminated);

    } else 
        current_node = front_node;
    
    return current_node->process->context;
}
        