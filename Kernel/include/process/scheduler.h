#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <process.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_PROC_COUNT         256
#define MAX_NAME_LENGTH        10
#define PROCESS_TABLE_MAX_SIZE 16

typedef struct process_entry {
    int pid;
    char name[MAX_NAME_LENGTH];
    int priority;
    void *stack;
    void *rbp;
    int children_count;
    char parent_name[MAX_NAME_LENGTH];
    int status;
} process_entry_t;

typedef struct process_table {
    process_entry_t entries[PROCESS_TABLE_MAX_SIZE];
    int count;
} process_table_t;

/**
 * @brief set current process in wait status
 *
 */
pid_t wait_process(pid_t pid, int *status_ptr);

/**
 * @brief Saves the CPU state in the current process and switches to the next
 * one in the queue. This method should be called while IF flag is set to 0.
 *
 */
extern void _force_schedule();

/**
 * @brief Switches the CPU to the context of the next process in the queue.
 * This method should be called while IF flag is set to 0.
 *
 * @param rsp stack pointer of current process to be saved
 * @return context_t* stack pointer value of new context
 */
context_t *schedule(context_t *rsp);

/**
 * @brief Creates and adds a new process to the queue. If this function is
 * called while a process is running, it assigns the created process as a child
 * to the current process.
 *
 * @param main main function of the process
 * @param argc argument count that the main function receives
 * @param argv an array containing all the arguments
 * @return int process id of created process, or -1 if process could not be
 * created.
 */
int add_process(function_t main, int argc, char *argv[]);

/**
 * @brief Removes current process from the queue and forces a schedule.
 *
 */
void exit_process(int status);

/**
 * @brief Removes process with the given pid from the queue.
 *
 * @param pid
 */
int kill_process(pid_t pid);

/**
 * @brief Get the current process that is running
 *
 * @return process_t* pointer to the current process
 */
process_t *get_current_process();

/**
 * @brief Get the process with the given pid.
 *
 * @param pid process id of the target node
 * @return process_t* pointer to the process with given pid, or NULL if the
 * process could not be found
 */
process_t *get_process(pid_t pid);

process_t *get_foreground_process();

void set_foreground_process(int pid);

void sleep(uint64_t channel);

// Returns pid of awakened process
int wakeup(uint64_t channel);

void init_scheduler();

int get_process_table(process_table_t *table);

#endif /* _SCHEDULER_H_ */