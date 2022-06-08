#ifndef _PROCESS_SCHEDULER_
#define _PROCESS_SCHEDULER_

#include <process/process.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PROC_COUNT   256

bool add_process(function_t main);

void exit_process();

process_t * get_current_process();

extern void _force_schedule();

uint64_t * schedule(uint64_t * rsp);

#endif