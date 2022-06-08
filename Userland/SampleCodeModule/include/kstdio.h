#include <stddef.h>
#include <stdint.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct time{
    uint64_t year;
    uint64_t month;
    uint64_t day;
    uint64_t hour;
    uint64_t minutes;
    uint64_t seconds;
} time_t;

int _read(int fd, char * buffer, size_t count);
int _write(int fd, char * buffer, size_t count);
int _switch_screen_mode(int mode);
int _clear_screen();
int _exit(int error_code);
int _time(time_t * time_struct, int utc_offset);
void _run(void *main);

char getchar();
int putchar (int character);
int puts(const char * str);

int printf(char * str, ...);
int scanf(char *str, ...);