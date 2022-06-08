#ifndef _PMM_H
#define _PMM_H

#include <stdint.h>
#include <stddef.h>

void init_pmm();

uint64_t * kmalloc(size_t size);

void kfree(uint64_t * ptr);

/**
 * temporary function for debugging
 */
void dump_mem();

#endif /* _PMM_H */