#ifndef _PROCESS_SCHEDULER_
#define _PROCESS_SCHEDULER_

#include <process/process.h>
#include <stdint.h>
#include <stdbool.h>

#define NPROC   10

bool add_process(process_t * process);

void exit_process();

process_t * get_current_process();

uint64_t * schedule(uint64_t * rsp);

#endif