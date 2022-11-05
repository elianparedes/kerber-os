#ifndef _KMMAN_H
#define _KMMAN_H

#include <stddef.h>

void * _malloc(size_t size);

void _free(void *ptr);

void _get_mem_state(int mem_state[]);

#endif