#include <idtLoader.h>
#include <pmm.h>
#include <scheduler.h>

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

int add_process(function_t main, char *arg) {
    if (process_count >= MAX_PROC_COUNT)
        return PID_ERR;

    process_t *process = new_process(main, arg);
    if (process == NULL)
        return PID_ERR;

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

static process_t *free_process(int pid) {
    node_t *aux_node = front_node;

    while (aux_node->next->process->pid != pid) aux_node = aux_node->next;

    node_t *target_node = aux_node->next;

    aux_node->next = target_node->next;

    if (rear_node == target_node)
        rear_node = target_node->next;

    if (front_node == target_node)
        front_node = target_node->next;

    // tells the parent that the children process ends
    if (target_node->process->parent->l_child == target_node->process)
        target_node->process->parent->l_child = NULL;

    if (target_node->process->parent->r_child == target_node->process)
        target_node->process->parent->r_child = NULL;

    kfree(target_node->process);
    kfree(target_node);

    process_count--;

    return target_node->process;
}

void exit_process() {

    free_process(current_node->process->pid);
    _force_schedule();
}

void kill_process(int pid) {

    process_t *target = free_process(pid);

    // the right way of doing this would be with signals and their handlers
    // between processes but these are not implemented yet.
    // since the shell doesn't know the graphics context of each process,
    // force printing messages should be done here.
    gprint_new_line(target->g_context);
    gprint_new_line(target->g_context);
    gprint_string("[ process terminated ]", target->g_context);
    gprint_new_line(target->g_context);

}

process_t *get_current_process() { return current_node->process; }

process_t *get_process(pid_t pid) {
    node_t *aux_node = front_node;

    while (aux_node->next->process->pid != pid) aux_node = aux_node->next;

    return aux_node->next->process;
}

uint64_t *schedule(uint64_t *rsp) {
    if (process_count == 0)
        return rsp;

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
