#include <kstdio.h>
#include <printmem.h>
#define PRINT_SIZE 32 // number of bytes

void printmem(uint8_t *address) {
    uint8_t buffer[PRINT_SIZE];
    uint16_t copied = _get_mem(address, buffer, PRINT_SIZE);
    uint8_t *current_address = address;
    for (int i = 0; i < copied; i++) {
        if (i % 8 == 0) {
            printf("\n");
            printf("line %d | 0x%x | ", (i / 8) + 1, current_address);
            current_address += 8;
        }
        uint8_t byte = buffer[i];
        if (byte <= 0xF) {
            printf("0%x ", byte);
        } else {
            printf("%x ", byte);
        }
    }
}