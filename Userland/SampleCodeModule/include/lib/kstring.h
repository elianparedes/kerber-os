#ifndef _KSTRING_H_
#define _KSTRING_H_

#include <stddef.h>

char *strchr(char *str, int character);
char *strcpy(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);
size_t strlen(const char *str);

void reverse(char str[], int length);

#endif