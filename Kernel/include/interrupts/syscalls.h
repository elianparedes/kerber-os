#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>
#include <realTimeClock.h>
#define ERROR -1
enum STD {STDIN, STDOUT, STDERR};
#define SYSCALL_READ 0
#define SYSCALL_WRITE 1
#define SYSCALL_GETTIME 96

uint16_t read(int fd, char * buffer, uint16_t count);
uint16_t write(int fd, char * buffer, uint16_t count);
uint8_t sys_gettime(time_t * struct_time, int utc_offset);

#endif