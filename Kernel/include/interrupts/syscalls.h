#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>
#include <rtc.h>

#define ERROR -1
#define SUCCESS 0

enum STD {STDIN, STDOUT, STDERR};

#define SYSCALL_READ 0
#define SYSCALL_WRITE 1
#define SYSCALL_SWITCH_SCREEN_MODE 4
#define SYSCALL_CLEAR_SCREEN 5
#define SYSCALL_CNTRL_LISTENER 6
#define SYSCALL_EXIT 60
#define SYSCALL_GETTIME 96
#define SYSCALL_RUN 66
#define SYSCALL_DELETE_CHAR 46

/**
 * @brief reads up to @count bytes from keyboard and copies them to @buffer
 * 
 * @param fd file descriptor (not implemented)
 * @param buffer structure to copy to
 * @param count number of bytes
 * @return uint16_t numbers of bytes read
 */
uint16_t read(int fd, char * buffer, uint16_t count);

/**
 * @brief writes up to @count bytes from @buffer and copies them to screen
 * 
 * @param fd file descriptor (not implemented)
 * @param buffer structure to write from
 * @param count number of bytes
 * @return uint16_t numbers of bytes written
 */
uint16_t write(int fd, char * buffer, uint16_t count);

uint8_t sys_gettime(time_t * struct_time, int utc_offset);

void sys_exit(int error_code);

void sys_switch_screen_mode(int mode);

void sys_clear_screen();

/**
 * @brief sets @listener pointer as control pressed flag
 * 
 * @param listener pointer to be used as flag
 * @return uint8_t SUCCESS if no error occured
 */
uint8_t sys_cntrl_listener(char * listener);

void sys_delete_char();

#endif
