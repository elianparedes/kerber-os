#include <interrupts/idtLoader.h>
#include <mem/pmm.h>
#include <process/scheduler.h>

#define PID_ERR -1
#define MAX_TERM_COUNT 2

typedef struct node node_t;

typedef struct node {
    process_t *process;
    node_t *next;
} node_t;

static node_t *current_node = NULL;
static node_t *front_node = NULL;
static node_t *rear_node = NULL;

static uint64_t process_count = 0;

static bool enqueue_process(process_t *process) {
    node_t *node = kmalloc(sizeof(node_t));
    if (node == NULL) return false;

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

int add_process(function_t main) {
    if (process_count >= MAX_PROC_COUNT) return PID_ERR;

    process_t *process = new_process(main);
    if (process == NULL) return PID_ERR;

    if (enqueue_process(process)) {
        if (current_node != NULL) {
            if (current_node->process->l_child == NULL)
                current_node->process->l_child = process;

            else if (current_node->process->r_child == NULL)
                current_node->process->r_child = process;
        }
        process->parent = current_node->process;
        return process->pid;
    }

    return PID_ERR;
}

void exit_process() {
    node_t *aux_node = front_node;

    while (aux_node->next != current_node) aux_node = aux_node->next;

    aux_node->next = current_node->next;

    if (rear_node == current_node) rear_node = current_node->next;

    if (front_node == current_node) front_node = current_node->next;

    if (current_node->process->parent->l_child->pid ==
        current_node->process->pid)
        current_node->process->parent->l_child = NULL;

    if (current_node->process->parent->r_child->pid ==
        current_node->process->pid)
        current_node->process->parent->r_child = NULL;

    kfree(current_node->process);
    kfree(current_node);

    // tells the parent that the children process ends

    process_count--;
    _force_schedule();
}

void kill_process(int pid) {
    node_t *aux_node = front_node;

    while (aux_node->next->process->pid != pid) aux_node = aux_node->next;

    node_t *target_node = aux_node->next;

    aux_node->next = target_node->next;

    if (rear_node == target_node) rear_node = target_node->next;

    if (front_node == target_node) front_node = target_node->next;

    if (target_node->process->parent->l_child->pid == target_node->process->pid)
        target_node->process->parent->l_child = NULL;

    if (target_node->process->parent->r_child->pid == target_node->process->pid)
        target_node->process->parent->r_child = NULL;

    kfree(target_node->process);
    kfree(target_node);

    process_count--;
}

process_t *get_current_process() { return current_node->process; }

process_t *get_process(pid_t pid) {
    node_t *aux_node = front_node;

    while (aux_node->next->process->pid != pid) 
        aux_node = aux_node->next;
        
    return  aux_node->next->process;
}

uint64_t *schedule(uint64_t *rsp) {
    if (process_count == 0) return rsp;

    if (current_node != NULL) {
        current_node->process->context = rsp;

        // skip paused processes
        do {
            current_node = current_node->next;
        } while (current_node->process->status == PAUSED);

    } else
        current_node = front_node;

    return current_node->process->context;
}
