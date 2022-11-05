// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "interrupts/time/time.h"
#include <circular_linked_list.h>
#include <fifo_queue.h>
#include <idtLoader.h>
#include <lib/linked_list.h>
#include <pmm.h>
#include <scheduler.h>

#define PID_ERR        -1
#define MAX_TERM_COUNT 2

static circular_list_t process_list;
static process_t *current_process;

static process_t *free_process(int pid);

static int search_by_status(process_t *process, pstatus_t status) {
    return process->status == status;
}

static int search_by_pid(process_t *process, pid_t pid) {
    return process->pid == pid;
}

static int search_by_channel(process_t *process, uint64_t channel) {
    return process->status == WAITING && process->channel == channel;
}

void init_scheduler() {
    process_list = new_circular_linked_list(search_by_pid);
}

int process_count() {
    return cl_size(process_list);
}

void wait_process() {
    // if no children, return
    if (size(current_process->children) == 0)
        return;

    while (1) {
        list_ptr children_list = current_process->children;

        process_t *terminated_child =
            find(children_list, TERMINATED, search_by_status);
        if (terminated_child != NULL) {
            free_process(terminated_child->pid);
            return;
        }

        sleep(current_process);
    }
}

void sleep(uint64_t channel) {
    current_process->channel = channel;
    current_process->status = WAITING;
    _force_schedule();
}

int wakeup(uint64_t channel) {
    process_t *target = cl_find(process_list, channel, search_by_channel);
    if (target == NULL)
        return PID_ERR;

    target->status = READY;
    target->channel = NULL;

    return target->pid;
}

int add_process(function_t main, int argc, char *argv[]) {
    if (process_count() >= MAX_PROC_COUNT)
        return PID_ERR;

    process_t *process = new_process(main, argc, argv);
    if (process == NULL)
        return PID_ERR;

    cl_add(process_list, process);

    if (current_process != NULL) {
        add(current_process->children, process);
        process->parent = current_process;
    }

    return process->pid;
}

static process_t *free_process(int pid) {
    process_t *target = cl_remove(process_list, pid);
    if (target == NULL)
        return NULL;

    // remove process from parent's children list
    remove(target->parent->children, pid);
    free_list(target->children);
    kfree(target);

    return target;
}

void exit_process() {
    close_dataDescriptor(current_process->dataDescriptors[0]);
    close_dataDescriptor(current_process->dataDescriptors[1]);

    if (current_process->pid > 0 && current_process->parent != NULL)
        wakeup(current_process->parent);

    // leave process as terminated. Parent will clean it up on wait
    current_process->status = TERMINATED;
    current_process->exit_status = 0;

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
    return current_process;
}

process_t *get_process(pid_t pid) {
    return cl_find(process_list, pid, search_by_pid);
}

context_t *schedule(context_t *rsp) {
    if (process_count() == 0)
        return rsp;

    if (current_process != NULL && current_process->status == READY &&
        ticks_elapsed() < current_process->priority)
        return rsp;

    if (current_process != NULL)
        current_process->context = rsp;

    do {
        current_process = cl_next(process_list);
    } while (current_process->status != READY);

    // set timer ticks to 0
    timer_reset();

    return current_process->context;
}
