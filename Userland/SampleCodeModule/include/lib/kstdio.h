#ifndef _KSTDIO_H_
#define _KSTDIO_H_

#include <stddef.h>
#include <stdint.h>

#define STDIN           0
#define STDOUT          1
#define STDERR          2

#define REG_SIZE        16
#define REGISTERS_COUNT 18

typedef enum chld {
    LEFT_CHD = 0,
    RIGHT_CHD
} chld_t;

typedef struct time {
    uint64_t year;
    uint64_t month;
    uint64_t day;
    uint64_t hour;
    uint64_t minutes;
    uint64_t seconds;
} time_rtc_t;

typedef struct cpu_state {
    uint64_t rsp;
    uint64_t rflags;
    uint64_t rip;

    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;

    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
} cpu_state_t;

typedef enum {
    KBD_PRINT_REG = 1
} request_t;

typedef enum pstatus {
    WAITING = 0,
    READY,
    TERMINATED,
} pstatus_t;

int _read(int fd, char *buffer, size_t count);
int _write(int fd, char *buffer, size_t count);
void _close(unsigned int fd);
int _switch_screen_mode(int mode);
int _clear_screen();
int _time(time_rtc_t *time_struct, int utc_offset);
int _cntrl_pressed();
int _copy_cpu_state(cpu_state_t *cpu_ptr, request_t request);
void _delete_char();
int _cntrl_listener(char *listener);
void _focus(int pid);
void _setfg(int pid);
int _get_mem(uint8_t *address, uint8_t *buffer, size_t count);
void _sched_yield();

// Process syscalls
int _run(void *main, int argc, char *argv[]);
int _exit(int error_code);
int _wait();
int _waitpid(int pid, int *status_ptr);
int _kill(int pid);
int _block(int pid);
int _unblock(int pid);
int _get_proc_status(int pid);
int _set_priority(int pid, int priority);
int _sleep(int seconds);
int _getpid();

int getchar();
int putchar(int character);
int puts(const char *str);

int printf(char *str, ...);
int scanf(char *str, ...);

#endif /* _KSTDIO_H_ */