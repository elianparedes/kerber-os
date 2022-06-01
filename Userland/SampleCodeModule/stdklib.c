#include "stdklib.h"

extern int _puts(const char * str, int length);

int puts(const char * str){
    size_t length= strlen(str);
    _puts(str, length);
    return 0;
}

size_t strlen(const char * str){
    int i=0;
    while (str[i]!=0){
        i++;
    }
    return i;
}