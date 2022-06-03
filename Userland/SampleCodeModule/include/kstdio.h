#include <stddef.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2

int _read(int fd, char * buffer, size_t count);
int _write(int fd, char * buffer, size_t count);

char getchar();
int putchar (int character);
int puts(const char * str);

int printf(char * str, ...);
int scanf(char *str, ...);