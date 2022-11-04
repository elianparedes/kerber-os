// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "interrupts/time/time.h"
#include <fifo_queue.h>
#include <idtLoader.h>
#include <pmm.h>
#include <scheduler.h>
#include <lib/linked_list.h>

#define PID_ERR        -1
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

static process_t *free_process(int pid);

static int process_wstatus(process_t *process, pstatus_t status) {
    return process->status == status;
}

void wait_process() {
    process_t *current_process = get_current_process();

    // if no children, return
    if (size(current_process->children) <= 0)
        return;

    while (1) {
        list_ptr children_list = current_process->children;

        process_t *terminated_child =
            find(children_list, PAUSED, process_wstatus);
        if (terminated_child != NULL) {
            free_process(terminated_child->pid);
            return;
        }

        sleep(current_process);
    }
}

void sleep(uint64_t channel) {
    process_t *current_process = get_current_process();
    current_process->channel = channel;
    current_process->status = PAUSED;
    _force_schedule();
}

int wakeup(uint64_t channel) {
    node_t *aux_node = front_node;

    do {
        if (aux_node->process->channel == channel) {
            aux_node->process->status = READY;
            aux_node->process->channel = NULL;
            return aux_node->process->pid;
        }

        aux_node = aux_node->next;
    } while (aux_node != front_node);
    return PID_ERR;
}

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

int add_process(function_t main, int argc, char *argv[]) {
    if (process_count >= MAX_PROC_COUNT)
        return PID_ERR;

    process_t *process = new_process(main, argc, argv);
    if (process == NULL)
        return PID_ERR;

    if (enqueue_process(process)) {
        if (current_node != NULL) {
            // add new process to children list of current process
            add(current_node->process->children, process);
            process->parent = current_node->process;
        }

        return process->pid;
    }

    return PID_ERR;
}
node_t *get_node_before(pid_t pid) {
    node_t *aux_node = front_node;

    do {
        if (aux_node->next->process->pid == pid)
            return aux_node;

        aux_node = aux_node->next;
    } while (aux_node != front_node);

    return NULL;
}

static process_t *free_process(int pid) {
    node_t *aux_node = get_node_before(pid);

    if (aux_node == NULL)
        return NULL;

    node_t *target_node = aux_node->next;

    aux_node->next = target_node->next;

    if (rear_node == target_node)
        rear_node = aux_node;

    if (front_node == target_node)
        front_node = target_node->next;

    // remove process from parent's children list
    remove(target_node->process->parent->children, pid);

    free_list(target_node->process->children);
    kfree(target_node->process);
    kfree(target_node);

    process_count--;

    return target_node->process;
}

void exit_process() {
    process_t *current_process = get_current_process();

    close_dataDescriptor(current_process->dataDescriptors[0]);
    close_dataDescriptor(current_process->dataDescriptors[1]);
    
    if (current_process->pid > 0 && current_process->parent != NULL)
        wakeup(current_process->parent);

    // leave process as terminated. Parent will clean it up on wait
    current_process->status = PAUSED;
    current_process->exit_status = 0;

    // free_process(current_process->pid);

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

process_t *get_current_process() {
    return current_node->process;
}

process_t *get_process(pid_t pid) {
    node_t *aux_node = front_node;

    do {
        if (aux_node->process->pid == pid)
            return aux_node->process;

        aux_node = aux_node->next;
    } while (aux_node != front_node);

    return NULL;
}

context_t *schedule(context_t *rsp) {
    if (process_count == 0)
        return rsp;

    /*if (current_node != NULL &&
        ticks_elapsed() < current_node->process->priority)
        return rsp;*/

    if (current_node != NULL) {
        current_node->process->context = rsp;

        // skip paused processes
        do {
            current_node = current_node->next;
        } while (current_node->process->status == PAUSED);

    } else
        current_node = front_node;

    // set timer ticks to 0
    //timer_reset();

    return current_node->process->context;
}
