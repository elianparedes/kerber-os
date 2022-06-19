#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <process/process.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_PROC_COUNT 256

int add_process(function_t main, char* arg);

void kill_process(pid_t pid);

void exit_process();

process_t* get_current_process();

process_t* get_process(pid_t pid);

extern void _force_schedule();

uint64_t* schedule(uint64_t* rsp);

#endif /* _SCHEDULER_H_ */