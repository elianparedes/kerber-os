// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>

#define MAX 10

void child2() {
    for (size_t i = 0; i < 3; i++) {
        printf("from child2: %d\n", i);
    }
}

void child() {
    for (size_t i = 0; i < 1; i++) {
        printf("from child1: %d\n", i);
        _run(child2, NULL, NULL);
        _wait();
    }
}

void sleeptest() {
    int pid = 0;
    int i = 0;
    while (i < MAX) {
        if ((pid = _run(child, NULL, NULL)) > 0) {
            printf("Process created %d\n", pid);
            printf("Sleeping...\n");
            _wait();
            printf("children terminated...\n");
        }
        i++;
    }
}