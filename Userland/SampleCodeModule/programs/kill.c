// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>
#include <kstring.h>
#include <test_util.h>

#define ERROR -1

int kill(int argc, char *argv[]) {
    if (argc < 2) {
        printf("kill: missing arguments\n");
        return -1;
    }
    if (argc > 2) {
        printf("kill: too many arguments\n");
        return -1;
    }
    int pid = satoi(argv[1]);
    if ((pid == 0 && strcmp("0", argv[1]) != 0) || pid < 0) {
        printf("kill: not a valid pid\n");
        return -1;
    }
    int status = _get_proc_status(pid);
    if (status == ERROR) {
        printf("kill: process does not exist\n");
        return -1;
    } else {
        _kill(pid);
    }

    return 0;
}