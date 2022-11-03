// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pmm.h>
#include <stdbool.h>

#define SIZE (8 * 1024 * 1024) // 8 MB
#define BASE_ADDR 0x600000

#define BUDDY
#ifdef BUDDY

// Buddy memory allocator.
// Implementation by bOS: https://github.com/camilaDiToro/bOS

#define ERROR -1
#define SUCCESS 0

#define MAX_LEVEL 23
#define MIN_LEVEL 8
#define BLOCKS (1 << (MAX_LEVEL - MIN_LEVEL + 1))

#define LEFT_CHILD(index) (2 * (index) + 1)
#define RIGHT_CHILD(index) (2 * (index) + 2)
#define PARENT(index) ((((index) + 1) >> 1) - 1)

typedef struct block
{
    uint8_t is_occupied;
    uint8_t subblocks;
} block;

static block *allocator; // array containing information of all memory blocks
static void *base_address = (void *)BASE_ADDR;
static void *heap_address;

static size_t occupied_mem = 0;

static void *block_to_ptr(unsigned int index, unsigned int level);

static int get_first_index(int level);
static int get_first_free_index(int level);

static int get_level(size_t requested_size);
static int get_max_level(void *ptr);

static int get_search_index(void *ptr, int max_level);
static int search_block(int index, int *level, void *ptr);

static void set_occupied(int index, int occupied);
static void update_parents(int index, int block);

static int get_first_index(int level)
{
    return ((1 << (MAX_LEVEL - level)) - 1);
}

static int get_first_free_index(int level)
{
    int index = get_first_index(level);
    int last_index = get_first_index(level - 1);

    while (index < last_index && (allocator[index].is_occupied || allocator[index].subblocks))
    {
        index++;
    }

    if (index == last_index)
    {
        return ERROR;
    }

    return index;
}

static void *block_to_ptr(unsigned int index, unsigned int level)
{
    return heap_address + (index - get_first_index(level)) * (1 << level);
}

// Given requested_size, returns minimum level that satisfies
static int get_level(size_t requested_size)
{
    requested_size--;
    int level = 0;
    while (requested_size != 0)
    {
        requested_size >>= 1;
        level++;
    }
    return level;
}

static int get_max_level(void *ptr)
{
    int relative_address = ptr - heap_address;
    int level = MAX_LEVEL;
    int pow_two = 1 << MAX_LEVEL;
    while ((relative_address % pow_two) != 0)
    {
        level--;
        pow_two = pow_two >> 1;
    }
    return level;
}

static int get_search_index(void *ptr, int max_level)
{
    int relative_address = ptr - heap_address;
    return get_first_index(max_level) + (relative_address / (1 << max_level));
}

static int search_block(int index, int *level, void *ptr)
{
    if (*level < MIN_LEVEL)
        return -1;

    if (!allocator[index].is_occupied)
    {
        (*level)--;
        int right_child_index = RIGHT_CHILD(index);
        void *address = block_to_ptr(right_child_index, *level);
        return search_block(ptr < address ? (LEFT_CHILD(index)) : right_child_index, level, ptr);
    }

    return ptr == block_to_ptr(index, *level) ? index : ERROR;
}

static void set_occupied(int index, int occupied)
{
    if (index < 0 || index >= BLOCKS)
        return;

    allocator[index].is_occupied = occupied;
    set_occupied(LEFT_CHILD(index), occupied);
    set_occupied(RIGHT_CHILD(index), occupied);
}

static void update_parents(int index, int block)
{
    allocator[index].subblocks += block;
    while (index != 0)
    {
        index = PARENT(index);
        allocator[index].subblocks += block;
    }
}

void init_pmm()
{
    // reserve space for allocator array
    allocator = base_address;
    size_t allocator_size = sizeof(block) * BLOCKS;
    base_address += allocator_size;
    // heap_address is now at 0x610000
    heap_address = base_address;

    for (int i = 0; i < BLOCKS; i++)
    {
        allocator[i].is_occupied = 0;
        allocator[i].subblocks = 0;
    }
}

void *kmalloc(size_t size)
{

    int level = get_level(size);
    if (level < MIN_LEVEL)
    {
        level = MIN_LEVEL;
    }

    int index = get_first_free_index(level);
    if (index == ERROR)
    {
        return NULL;
    }

    set_occupied(index, 1);
    update_parents(index, 1);

    occupied_mem += (1 << level);

    return block_to_ptr(index, level);
}

void kfree(void *ptr)
{
    if (ptr == NULL || ptr > heap_address || ptr > (heap_address + SIZE))
        return;

    int level = get_max_level(ptr);
    int index = search_block(get_search_index(ptr, level), &level, ptr);

    if (index < 0)
        return;

    update_parents(index, -1);
    set_occupied(index, 0);

    occupied_mem -= (1 << level);
}

void get_mem_info(int mem_state[])
{
    mem_state[0] = SIZE;                // total memory in bytes
    mem_state[1] = occupied_mem;        // occupied memory
    mem_state[2] = SIZE - occupied_mem; // free memory
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