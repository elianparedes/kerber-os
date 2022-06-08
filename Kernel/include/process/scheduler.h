#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <process/process.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PROC_COUNT   256

bool add_process(function_t main);

void exit_process();

process_t * get_current_process();

extern void _force_schedule();

uint64_t * schedule(uint64_t * rsp);

#endif /* _SCHEDULER_H_ */