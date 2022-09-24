// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>
#include <kstdlib.h>
#include <printmem.h>
#define PRINT_SIZE 32 // number of bytes

void printmem(char *arg) {
    uint8_t buffer[PRINT_SIZE];
    char *endp;
    long address = strtol(arg, &endp, 16);

    // if endpointer isn't zero,
    // then the argument is not a hexadecimal number
    if (*endp != '\0') {
        printf("Invalid argument.\n");
        return;
    }

    uint16_t copied = _get_mem((uint8_t *)address, buffer, PRINT_SIZE);
    char *current_address = (char *)address;
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