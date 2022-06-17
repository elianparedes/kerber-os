#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <rtc.h>
#include <registers.h>

#define ERROR -1

enum STD {STDIN, STDOUT, STDERR};

#define SYSCALL_READ 0
#define SYSCALL_WRITE 1
#define SYSCALL_SWITCH_SCREEN_MODE 4
#define SYSCALL_CLEAR_SCREEN 5
#define SYSCALL_CNTRL_PRESSED 6
#define SYSCALL_COPY_CPU_STATE 7
#define SYSCALL_EXIT 60
#define SYSCALL_GETTIME 96
#define SYSCALL_RUN 66

uint16_t read(int fd, char * buffer, uint16_t count);
uint16_t write(int fd, char * buffer, uint16_t count);
uint8_t sys_gettime(time_t * struct_time, int utc_offset);

void sys_exit(int error_code);
void sys_switch_screen_mode(int mode);
int sys_copy_cpu_state(cpu_state_t* cpu_ptr);
int sys_cntrl_pressed();

#endif
