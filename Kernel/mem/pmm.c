// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h> // temporary import
#include <pmm.h>
#include <stddef.h>

#define SIZE      (1024 * 64)
#define BASE_ADDR 0x600000

typedef union header {
    uint64_t size;
    uint8_t alloced : 1; // bit_0
} __attribute__((packed)) header_t;

static header_t *start = (header_t *)BASE_ADDR;

void init_pmm() {
    header_t *current_header = start;

    current_header->size = SIZE;
    current_header->alloced = 0;

    // insert the end of space header

    current_header =
        current_header + 1 + (current_header->size & ~0x1) / sizeof(header_t);
    current_header->size = 0;
    current_header->alloced = 1;
}

void *kmalloc(size_t size) {
    header_t *current_header = start;

    while (((uint64_t)current_header < BASE_ADDR + SIZE - sizeof(header_t)) &&
           (current_header->size & ~0x1) > 0) {
        /**
         * header_t is an union and the first bit represents if is allocated or
         * not when reading size we must mask out the alloced flag
         */
        if (size <= (current_header->size & ~0x1) && !current_header->alloced) {
            void *alloced_addr = current_header + 1; // return pointer
            uint64_t old_size = current_header->size & ~0x1;
            uint64_t new_size = size & ~0x1; // round up to even value

            current_header->size = new_size;
            current_header->alloced = 1;

            // then insert new split block

            if (new_size < old_size) {
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

void kfree(void *ptr) {
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
