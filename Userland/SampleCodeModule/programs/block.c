// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>
#include <test_util.h>
#include <kstring.h>

#define ERROR -1

int block(int argc, char *argv[]){
    if (argc < 2){
        printf("block: missing arguments\n");
        return -1;
    }
    if (argc > 2){
        printf("block: too many arguments\n");
        return -1;
    }
    int pid = satoi(argv[1]);
    if ((pid == 0 && strcmp("0", argv[1]) != 0) || pid < 0){
        printf("block: not a valid pid\n");
        return -1;
    }
    int status = _get_proc_status(pid);
    switch(status){
        case READY:
            _block(pid);
            break;
        case WAITING:
            _unblock(pid);
            break;
        case ERROR:
            printf("block: process does not exist\n");
            break;
        default:
            break;
    }

    return 0;
}