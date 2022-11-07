// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>
#include <schd.h>

int dummy(int argc, char *argv[]) {
    return 0;
}

void schd() {
    char *args[] = {"dummy"};
    while (1) {
        _run(dummy, 1, args);
        _run(dummy, 1, args);
        _wait();
        _wait();
    }

    return 0;
}