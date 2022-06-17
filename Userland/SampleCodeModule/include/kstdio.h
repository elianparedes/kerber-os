#ifndef _KSTDIO_H_
#define _KSTDIO_H_

#include <stddef.h>
#include <stdint.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define REG_SIZE 16
#define REGISTERS_COUNT 18

typedef struct time{
    uint64_t year;
    uint64_t month;
    uint64_t day;
    uint64_t hour;
    uint64_t minutes;
    uint64_t seconds;
} time_t;

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

int _read(int fd, char * buffer, size_t count);
int _write(int fd, char * buffer, size_t count);
int _switch_screen_mode(int mode);
int _clear_screen();
int _exit(int error_code);
int _time(time_t * time_struct, int utc_offset);
void _run(void *main);
int _cntrl_pressed();
int _copy_cpu_state(cpu_state_t * cpu_ptr);

char getchar();
int putchar (int character);
int puts(const char * str);

int printf(char * str, ...);
int scanf(char *str, ...);

#endif /* _KSTDIO_H_ */