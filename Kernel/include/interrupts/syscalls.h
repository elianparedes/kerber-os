#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>
#include <rtc.h>

#define ERROR -1

enum STD {STDIN, STDOUT, STDERR};

#define SYSCALL_READ 0
#define SYSCALL_WRITE 1
#define SYSCALL_SWITCH_SCREEN_MODE 4
#define SYSCALL_CLEAR_SCREEN 5
#define SYSCALL_CNTRL_PRESSED 6
#define SYSCALL_EXIT 60
#define SYSCALL_GETTIME 96
#define SYSCALL_RUN 66
#define SYSCALL_DELETE_CHAR 46

uint16_t read(int fd, char * buffer, uint16_t count);
uint16_t write(int fd, char * buffer, uint16_t count);
uint8_t sys_gettime(time_t * struct_time, int utc_offset);

void sys_exit(int error_code);

void sys_switch_screen_mode(int mode);

void sys_clear_screen();

int sys_cntrl_pressed();

void sys_delete_char();

#endif
