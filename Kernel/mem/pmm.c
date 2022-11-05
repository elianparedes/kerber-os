// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pmm.h>
#include <stdbool.h>

#define ERROR -1
#define SUCCESS 0

#define SIZE (1024 * 64)
#define BASE_ADDR 0x600000

#define BUDDY
#ifdef BUDDY

#define ALIGNMENT 16

typedef struct buddy_block
{
    size_t size;
    bool is_free;
    struct buddy_block * next;
    struct buddy_block * previous;
} buddy_block;

typedef struct
{
    buddy_block *head; // base address
    buddy_block *tail; // not real buddy block
    size_t alignment;
} buddy_allocator;

static buddy_block *buddy_find_best(buddy_block *head, buddy_block *tail, size_t size);
static void buddy_merge(buddy_block *head, buddy_block *tail);
static buddy_block *buddy_next(buddy_block *block);
static void buddy_insert_after(buddy_block * block, buddy_block * new_block);
static void buddy_remove_block(buddy_block * block);
static buddy_allocator b;
static buddy_allocator *buddy_alloc = &b;

static void buddy_insert_after(buddy_block * block, buddy_block * new_block){
    buddy_block * next_block = block->next;
    block->next=new_block;
    new_block->next=next_block;
    new_block->previous=block;
    next_block->previous=new_block;
}

static void buddy_remove_block(buddy_block * block){
    buddy_block * prev_block=block->previous;
    buddy_block * next_block=block->next;
    prev_block->next=next_block;
    next_block->previous=prev_block;
}

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
    buddy_block *buddy = buddy_next(block);

    if (buddy == tail && block->is_free)
    {
        return buddy_split(block, size);
    }

    while (block < tail && buddy < tail)
    {

        // Found available block left side
        if (block->is_free && size <= block->size &&
            (best_block == NULL || block->size <= best_block->size))
        {
            best_block = block;
        }

        // Found available block right side
        if (buddy->is_free && size <= buddy->size &&
            (best_block == NULL || buddy->size < best_block->size))
        {
            best_block = buddy;
        }

        block = buddy;
        buddy = buddy_next(buddy);

        /*

        if (block->size <= buddy->size)
        {
            block = buddy_next(buddy);
            if (block < tail)
            {
                buddy = buddy_next(block);
            }
        }
        else
        {
            block = buddy;
            buddy = buddy_next(buddy);
        }
        */
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
                    // Leave the buddy block for the next iteration
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

uintptr_t align_forward(uintptr_t ptr, size_t align)
{
    uintptr_t p, a, modulo;

    // assert(is_power_of_two(align));

    p = ptr;
    a = (uintptr_t)align;
    // Same as (p % a) but faster as 'a' is a power of two
    modulo = p & (a - 1);

    if (modulo != 0)
    {
        // If 'p' address is not aligned, push the address to the
        // next value which is aligned
        p += a - modulo;
    }
    return p;
}

size_t buddy_block_size_required(size_t size)
{
    size_t actual_size = buddy_alloc->alignment;

    size += sizeof(buddy_block);
    size = align_forward(size, buddy_alloc->alignment);

    while (size > actual_size)
    {
        actual_size <<= 1;
    }

    return actual_size;
}

void init_pmm()
{
    buddy_alloc->alignment = ALIGNMENT;
    if (buddy_alloc->alignment < sizeof(buddy_block)) {
        buddy_alloc->alignment = sizeof(buddy_block);
    }
    
    buddy_alloc->head = (buddy_block *)BASE_ADDR;
    buddy_alloc->head->size = (size_t)SIZE;
    buddy_alloc->head->is_free = true;
    buddy_alloc->alignment = ALIGNMENT;

    buddy_alloc->tail = buddy_next(buddy_alloc->head);
}

void *kmalloc(size_t size)
{
    if (size != 0)
    {
        size_t actual_size = buddy_block_size_required(size);

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
        buddy_block * block = (buddy_block *)((char *)ptr - buddy_alloc->alignment);
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