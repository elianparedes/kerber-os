// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>

#define MAX 10

void child() {
    for (size_t i = 0; i < MAX; i++) {
        printf("Print: %d\n", i);
    }
}

void sleeptest() {
    int pid = 0;
    while (1) {
        if ((pid = _run(child, NULL)) > 0) {
            printf("Process created %d\n", pid);
            printf("Sleeping...\n");
            _wait2();
            printf("Opa...\n");
        }
    }
}