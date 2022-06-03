#include <stddef.h>
#include <stdarg.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2

char getchar();
int putchar (int character);
int puts(const char * str);
size_t strlen(const char * str);

//--https://www.geeksforgeeks.org/implement-itoa/--
char * itoa(int num, char* str, int base);
//-------------------------------------------------

int printf(char * str, ...);
int scanf(char *str, ...);