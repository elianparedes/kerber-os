#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

void init_pmm();

uint64_t * kmalloc(size_t size);

void kfree(uint64_t * ptr);

/**
 * temporary function for debugging
 */
void dump_mem();

#endif