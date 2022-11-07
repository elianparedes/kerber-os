#ifndef _KSTDLIB_H_
#define _KSTDLIB_H_

#include <stddef.h>
#include <stdint.h>

#define EOF -1

char *itoa(int num, char *str, int base);
char *ltoa(long num, char *str, int base);
long strtol(const char *nptr, char **endptr, int base);

#endif /* _KSTDLIB_H_ */