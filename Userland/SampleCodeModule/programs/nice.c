// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>
#include <kstring.h>
#include <nice.h>
#include <test_util.h>

#define ERROR -1

void nice(int argc, char *argv[]) {
    if (argc < 3) {
        printf("nice: missing arguments\n");
        return;
    }
    if (argc > 3) {
        printf("nice: too many arguments\n");
        return;
    }
    int pid = satoi(argv[1]);
    if ((pid == 0 && strcmp("0", argv[1]) != 0) || pid < 0) {
        printf("nice: not a valid pid\n");
        return;
    }
    int priority = satoi(argv[2]);
    int ret = _set_priority(pid, priority);
    if (ret == ERROR) {
        printf("nice: process does not exist\n");
    }
}