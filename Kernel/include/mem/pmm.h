#ifndef _PMM_H
#define _PMM_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief initializes de physical memory manager, the heap where all memory
 * blocks are going to be allocated and the tracking data structure.
 *
 */
void init_pmm();

/**
 * @brief Allocates the requested memory and returns a pointer to it.
 *
 * @param size size of the memory block, in bytes.
 * @return uint64_t* pointer to the allocated memory, or NULL if the request
 * fails.
 */
uint64_t *kmalloc(size_t size);

/**
 * @brief Deallocates the memory previously allocated by a call kmalloc.
 *
 * @param ptr pointer to a memory block previously allocated with kmalloc.
 */
void kfree(uint64_t *ptr);

/**
 * @brief Dumps the blocks allocated in memory on the screen for debugging
 * purposes.
 *
 */
void dump_mem();

#endif /* _PMM_H */