#ifndef _LIB_H_
#define _LIB_H_

/* Kernel utilities */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <syscalls.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

char *cpuVendor(char *result);

void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);

int puts(const char *str);

char *itoa(int num, char *str, int base);

int printf(char *str, ...);

size_t strlen(const char *str);

char *strcat(char *destination, const char *source);

#endif /* _LIB_H_ */