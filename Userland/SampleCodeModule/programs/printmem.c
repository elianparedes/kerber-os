#include <kstdio.h>
#include <kstdlib.h>
#include <printmem.h>
#define PRINT_SIZE 32 // number of bytes

void printmem(char *arg) {
    uint8_t buffer[PRINT_SIZE];
    uint8_t *endp;
    uint8_t *address = (uint8_t *)strtol(arg, (char **)&endp, 16);

    // if endpointer isn't zero,
    // then the argument is not a hexadecimal number
    if (*endp != '\0') {
        printf("Invalid argument.\n");
        return;
    }

    uint16_t copied = _get_mem(address, buffer, PRINT_SIZE);
    uint8_t *current_address = address;
    for (int i = 0; i < copied; i++) {
        if (i % 8 == 0) {
            printf("%s0x%x | ", i == 0 ? "" : "\n", current_address);
            current_address += 8;
        }
        uint8_t byte = buffer[i];
        if (byte <= 0xF) {
            printf("0%x ", byte);
        } else {
            printf("%x ", byte);
        }
    }
    printf("\n");
}