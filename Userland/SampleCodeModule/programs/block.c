#include <kstdio.h>
#include <test_util.h>
#include <kstring.h>

#define ERROR -1

void block(int argc, char ** argv){
    if (argc == 0){
        printf("block: missing arguments\n");
        return;
    }
    if (argc > 1){
        printf("block: too many arguments\n");
        return;
    }
    int pid = satoi(argv[0]);
    if ((pid == 0 && strcmp("0", argv[0]) != 0) || pid < 0){
        printf("block: not a valid pid\n");
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
}