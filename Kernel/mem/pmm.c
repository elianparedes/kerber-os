// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pmm.h>
#include <stdbool.h>

#define SIZE (1024 * 64)
#define BASE_ADDR 0x600000

#define BUDDY
#ifdef BUDDY

/*
Buddy memory allocator. Based on GingerBill's implementation: 
https://www.gingerbill.org/article/2021/12/02/memory-allocation-strategies-006/

SIZE and ALIGNMENT must be powers of two for the buddy to work correctly
*/

#define ALIGNMENT 16

typedef struct buddy_block
{
	size_t size;
	bool is_free;
} buddy_block;

typedef struct
{
	buddy_block *head; // base address
	buddy_block *tail; // not real buddy block
	size_t alignment;
} buddy_allocator;

static buddy_allocator b;
static buddy_allocator *buddy_alloc = &b;

static buddy_block *buddy_find_best(buddy_block *head, buddy_block *tail, size_t size);
static void buddy_merge(buddy_block *head, buddy_block *tail);
static buddy_block *buddy_split(buddy_block *block, size_t size);

static buddy_block *buddy_next(buddy_block *block);

static size_t buddy_align_size(size_t size);

static buddy_block *buddy_next(buddy_block *block)
{
	return (buddy_block *)((char *)block + block->size);
}

static buddy_block *buddy_split(buddy_block *block, size_t size)
{
	if (block != NULL && size != 0)
	{
		while (block->size > size)
		{
			size_t sz = block->size >> 1;
			block->size = sz;
			block = buddy_next(block);
			block->size = sz;
			block->is_free = true;
		}

		if (size <= block->size)
		{
			return block;
		}
	}

	return NULL;
}

static buddy_block *buddy_find_best(buddy_block *head, buddy_block *tail, size_t size)
{
	buddy_block *best_block = NULL;
	buddy_block *block = head;

	if (buddy_next(block)== tail && block->is_free)
	{
		return buddy_split(block, size);
	}

	while (block < tail)
	{

		if (block->is_free && size <= block->size &&
			(best_block == NULL || block->size <= best_block->size))
		{
			best_block = block;
		}

		block = buddy_next(block);
	}

	if (best_block != NULL)
	{
		return buddy_split(best_block, size);
	}

	return NULL;
}

static void buddy_merge(buddy_block *head, buddy_block *tail)
{
	while (true)
	{
		buddy_block *block = head;
		buddy_block *buddy = buddy_next(block);

		bool no_merge = true;
		while (block < tail && buddy < tail)
		{
			// Merge two blocks together
			if (block->is_free && buddy->is_free && block->size == buddy->size)
			{
				block->size <<= 1;
				block = buddy_next(block);
				if (block < tail)
				{
					buddy = buddy_next(block);
					no_merge = false;
				}
			}
			else if (block->size < buddy->size)
			{
				// keep looking
				block = buddy;
				buddy = buddy_next(buddy);
			}
			else
			{
				block = buddy_next(buddy);
				if (block < tail)
				{
					buddy = buddy_next(block);
				}
			}
		}

		if (no_merge)
		{
			return;
		}
	}
}

static size_t buddy_align_size(size_t size)
{
	size_t actual_size = buddy_alloc->alignment;

	size += sizeof(buddy_block);
	//size += (size % buddy_alloc->alignment);

	while (size > actual_size)
	{
		actual_size <<= 1;
	}

	return actual_size;
}

void init_pmm()
{
	buddy_alloc->alignment = ALIGNMENT;
	if (buddy_alloc->alignment < sizeof(buddy_block))
	{
		buddy_alloc->alignment = sizeof(buddy_block);
	}

	buddy_alloc->head = (buddy_block *)BASE_ADDR;
	buddy_alloc->head->size = (size_t)SIZE;
	buddy_alloc->head->is_free = true;

	buddy_alloc->tail = buddy_next(buddy_alloc->head);
}

void *kmalloc(size_t size)
{
	if (size != 0)
	{
		size_t actual_size = buddy_align_size(size);

		buddy_block *found = buddy_find_best(buddy_alloc->head, buddy_alloc->tail, actual_size);
		if (found == NULL)
		{
			buddy_merge(buddy_alloc->head, buddy_alloc->tail);
			found = buddy_find_best(buddy_alloc->head, buddy_alloc->tail, actual_size);
		}

		if (found != NULL)
		{
			found->is_free = false;
			return (void *)((char *)found + buddy_alloc->alignment);
		}
	}

	return NULL;
}

void kfree(void *ptr)
{
	if (ptr != NULL)
	{
		buddy_block *block = (buddy_block *)((char *)ptr - buddy_alloc->alignment);
		block->is_free = true;
	}
}

#else

typedef union header
{
	uint64_t size;
	uint8_t alloced : 1; // bit_0
} __attribute__((packed)) header_t;

static header_t *start = (header_t *)BASE_ADDR;

void init_pmm()
{
	header_t *current_header = start;

	current_header->size = SIZE;
	current_header->alloced = 0;

	// insert the end of space header

	current_header =
		current_header + 1 + (current_header->size & ~0x1) / sizeof(header_t);
	current_header->size = 0;
	current_header->alloced = 1;
}

void *kmalloc(size_t size)
{
	header_t *current_header = start;

	while (((uint64_t)current_header < BASE_ADDR + SIZE - sizeof(header_t)) &&
		   (current_header->size & ~0x1) > 0)
	{
		/**
		 * header_t is an union and the first bit represents if is allocated or
		 * not when reading size we must mask out the alloced flag
		 */
		if (size <= (current_header->size & ~0x1) && !current_header->alloced)
		{
			void *alloced_addr = current_header + 1; // return pointer
			uint64_t old_size = current_header->size & ~0x1;
			uint64_t new_size = size & ~0x1; // round up to even value

			current_header->size = new_size;
			current_header->alloced = 1;

			// then insert new split block

			if (new_size < old_size)
			{
				// go to the next header
				current_header =
					current_header + 1 + new_size / sizeof(header_t);

				current_header->size = old_size - new_size - sizeof(header_t);
				current_header->alloced = 0;
			}

			return alloced_addr;
		}

		// go to the next header
		current_header = current_header + 1 +
						 (current_header->size & ~0x1) / sizeof(header_t);
	}
	return NULL;
}

void kfree(void *ptr)
{
	header_t *current_header = (header_t *)ptr;

	current_header = current_header - 1;
	((header_t *)current_header)->alloced = 0;

	header_t *next_header = current_header + 1 +
							(current_header->size & ~0x1) /
								sizeof(header_t); // go to the next header
	if (!next_header->alloced)
		current_header->size +=
			next_header->size + sizeof(header_t); // no need to mask out alloced
												  // bit because is always 0
}

/*void dump_mem() {
	header_t *current_header = start;

	while ((current_header->size & ~0x1) > 0) {
		ncPrint("[ ");
		ncPrintHex(current_header);
		ncPrintChar(' ');
		ncPrintHex(current_header->size & ~0x1);
		ncPrintChar(' ');
		ncPrintDec(current_header->alloced);
		ncPrint(" ] ");

		current_header = current_header + 1 +
						 (current_header->size & ~0x1) / sizeof(header_t);
	}


	//print end header

	ncPrint("[ ");
	ncPrintHex(current_header);
	ncPrintChar(' ');
	ncPrintHex(current_header->size & ~0x1);
	ncPrintChar(' ');
	ncPrintDec(current_header->alloced);
	ncPrint(" ] ");
}*/

#endif