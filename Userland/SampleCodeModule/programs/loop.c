// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>
#include <test_util.h>

int loop(int argc, char *argv[]) {
    int seconds;
    if (argc != 2) {
        printf("loop: missing arguments in seconds\n");
        return -1;
    }

    if ((seconds = satoi(argv[1])) <= 0) {
        printf("loop: please enter a valid value in seconds\n");
        return -1;
    }

    while (1) {
        printf("Hello World!\n");
        _sleep(seconds);
    }
}