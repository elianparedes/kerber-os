#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <rtc.h>
#include <registers.h>

#define ERROR -1
#define SUCCESS 0

enum STD {STDIN, STDOUT, STDERR};

#define SYSCALL_READ 0
#define SYSCALL_WRITE 1
#define SYSCALL_SWITCH_SCREEN_MODE 4
#define SYSCALL_CLEAR_SCREEN 5

#define SYSCALL_CNTRL_LISTENER 6
#define SYSCALL_GET_MEM 10
#define SYSCALL_COPY_CPU_STATE 7

#define SYSCALL_EXIT 60
#define SYSCALL_GETTIME 96
#define SYSCALL_RUN 66
#define SYSCALL_DELETE_CHAR 46
#define SYSCALL_KILL 62
#define SYSCALL_PAUSE 75
#define SYSCALL_RUNNING 67

/**
 * @brief reads up to @count bytes from keyboard and copies them to @buffer
 * 
 * @param fd file descriptor (not implemented)
 * @param buffer structure to copy to
 * @param count number of bytes to read
 * @return uint16_t numbers of bytes read
 */
uint16_t read(int fd, char * buffer, uint16_t count);

/**
 * @brief writes up to @count bytes from @buffer and copies them to screen
 * 
 * @param fd file descriptor (not implemented)
 * @param buffer structure to write from
 * @param count number of bytes to write
 * @return uint16_t numbers of bytes written
 */
uint16_t write(int fd, char * buffer, uint16_t count);

/**
 * @brief fills @struct_time given with time information 
 * 
 * @param struct_time struct pointer containing year, month, day, hour, minutes and seconds
 * @param utc_offset offset to change hour
 * @return uint8_t SUCCESS if no error occured
 */
uint8_t sys_gettime(time_t * struct_time, int utc_offset);

void sys_exit(int error_code);

void sys_switch_screen_mode(int mode);

int sys_copy_cpu_state(cpu_state_t* cpu_ptr,request_t request);


void sys_clear_screen();

/**
 * @brief sets @listener pointer as control pressed flag
 * 
 * @param listener pointer to be used as flag
 * @return uint8_t SUCCESS if no error occured
 */
uint8_t sys_cntrl_listener(char * listener);

void sys_delete_char();

int sys_run(void *main);

int sys_running(int pid);

void sys_kill(int pid);

/**
 * @brief copies memory starting from @address up to @count bytes to @buffer
 * 
 * @param address pointer to a memory address
 * @param buffer structure to write to
 * @param count number of bytes to copy
 * @return uint16_t number of bytes copied to buffer
 */
uint16_t sys_get_mem(uint8_t * address, uint8_t * buffer, uint16_t count);

void sys_pause(int pid);

#endif
