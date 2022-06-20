#ifndef _LIB_H_
#define _LIB_H_

/* Kernel utilities */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <syscalls.h>

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param destination
 * @param character
 * @param length
 * @return void*
 */
void *memset(void *destination, int32_t character, uint64_t length);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param destination
 * @param source
 * @param length
 * @return void*
 */
void *memcpy(void *destination, const void *source, uint64_t length);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param result
 * @return char*
 */
char *cpuVendor(char *result);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param port
 * @param value
 */
void outb(uint16_t port, uint8_t value);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param port
 * @return uint8_t
 */
uint8_t inb(uint16_t port);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param str
 * @return int
 */
int puts(const char *str);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param num
 * @param str
 * @param base
 * @return char*
 */
char *itoa(int num, char *str, int base);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param str
 * @param ...
 * @return int
 */
int printf(char *str, ...);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param str
 * @return size_t
 */
size_t strlen(const char *str);

/**
 * TODO: Fill function comments
 * @brief
 *
 * @param destination
 * @param source
 * @return char*
 */
char *strcat(char *destination, const char *source);

#endif /* _LIB_H_ */