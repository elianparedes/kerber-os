#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <registers.h>
#include <rtc.h>
#include <stdint.h>
#include <stddef.h>

#define ERROR   -1
#define SUCCESS 0

enum STD {
    STDIN,
    STDOUT,
    STDERR
};

#define SYSCALL_READ               0
#define SYSCALL_WRITE              1
#define SYSCALL_SWITCH_SCREEN_MODE 4
#define SYSCALL_CLEAR_SCREEN       5

#define SYSCALL_CNTRL_LISTENER     6
#define SYSCALL_GET_MEM            10
#define SYSCALL_COPY_CPU_STATE     7
#define SYSCALL_MALLOC 9
#define SYSCALL_FREE 11

#define SYSCALL_EXIT               60
#define SYSCALL_GETTIME            96
#define SYSCALL_RUN                66
#define SYSCALL_DELETE_CHAR        46
#define SYSCALL_KILL               62
#define SYSCALL_PAUSE              75
#define SYSCALL_WAIT               67
#define SYSCALL_WAIT2              68
#define SYSCALL_FOCUS              77
#define SYSCALL_SCHED_YIELD 24

#define SYSCALL_SEM_OPEN 70
#define SYSCALL_SEM_WAIT 71
#define SYSCALL_SEM_POST 72
#define SYSCALL_SEM_CLOSE 73
#define SYSCALL_GET_SEMS 74

typedef struct sem * sem_ptr;
typedef struct copy_sem copy_sem_t;

/**
 * @brief Reads up to count bytes from keyboard and copies them to buffer
 *
 * @param fd file descriptor (not implemented)
 * @param buffer structure to copy to
 * @param count number of bytes to read
 * @return uint16_t numbers of bytes read
 */
uint16_t read(int fd, char *buffer, uint16_t count);

/**
 * @brief Writes up to count bytes from buffer and copies them to screen
 *
 * @param fd file descriptor (not implemented)
 * @param buffer structure to write from
 * @param count number of bytes to write
 * @return uint16_t numbers of bytes written
 */
uint16_t write(int fd, char *buffer, uint16_t count);

/**
 * @brief Fills struct_time given with time information
 *
 * @param struct_time struct pointer containing year, month, day, hour, minutes
 * and seconds
 * @param utc_offset offset to change hour
 * @return uint8_t SUCCESS if no error occured
 */
uint8_t sys_gettime(time_t *struct_time, int utc_offset);

void sys_exit(int error_code);

/**
 * @brief Calls the graphics manager to switch the screen mode, clear the screen
 * and reset the cursor.
 *
 * @param mode either 0 or 1 for fullscreen or split screen respectively.
 */
void sys_switch_screen_mode(int mode);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param cpu_ptr
 * @param request
 * @return int
 */
int sys_copy_cpu_state(cpu_state_t *cpu_ptr, request_t request);

/**
 * @brief Calls the graphics manager to clear the screen
 *
 */
void sys_clear_screen();

/**
 * @brief Sets listener pointer as control pressed flag
 *
 * @param listener pointer to be used as flag
 * @return uint8_t SUCCESS if no error occured
 */
uint8_t sys_cntrl_listener(uint8_t *listener);

/**
 * @brief Calls the graphics manager to delete a character from screen and move
 * the cursor back
 */
void sys_delete_char();

/**
 * @brief Calls the scheduler to create and enqueue a new process.
 *
 * @param main main function of the new process
 * @param arg argument that the main function receives
 * @return int process id of the created process
 */
int sys_run(void *main, char *arg);

/**
 * @brief Puts the value passed by reference in 0 or 1 depending if
 * given child is terminated or running respectively.
 *
 * @param child either 0 or 1 for left and right child respectively
 * @param pstatus pointer to write to
 */
void sys_wait(int child, int *pstatus);

/**
 * @brief Terminates the execution of given process
 *
 * @param pid process id of the target process
 */
void sys_kill(int pid);

/**
 * @brief Copies memory starting from address up to @count bytes to buffer
 *
 * @param address pointer to a memory address
 * @param buffer structure to write to
 * @param count number of bytes to copy
 * @return uint16_t number of bytes copied to buffer
 */
uint16_t sys_get_mem(uint8_t *address, uint8_t *buffer, uint16_t count);

/**
 * @brief Pauses the execution of given process
 *
 * @param pid process id of the target process
 */
void sys_pause(int pid);

/**
 * @brief Calls the graphics manager to move the focus indicator
 * to the given process window on screen
 *
 * @param pid process id of the target process
 */
void sys_focus(int pid);

void sys_sched_yield();

void sys_wait2();

//Semaphore syscalls
sem_ptr sys_sem_open(char * name, int value);

int sys_sem_wait(sem_ptr sem);

int sys_sem_post(sem_ptr sem);

int sys_sem_close(sem_ptr sem);

int sys_get_semaphores(copy_sem_t * sems[]);

void * sys_malloc(size_t size);

void sys_free(void *ptr);

#endif
