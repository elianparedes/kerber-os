#include <stdint.h>
#define ERROR -1
enum STD {STDIN, STDOUT, STDERR};
#define SYSCALL_READ 0
#define SYSCALL_WRITE 1

uint16_t read(int fd, char * buffer, uint16_t count);
uint16_t write(int fd, char * buffer, uint16_t count);
