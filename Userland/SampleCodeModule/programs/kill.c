#include <kstdio.h>

#define ERROR -1

void kill(int argc, char * argv[]){
    if (argc < 2){
        printf("kill: missing arguments\n");
        return;
    }
    if (argc > 2){
        printf("kill: too many arguments\n");
        return;
    }
    int pid = satoi(argv[1]);
    if ((pid == 0 && strcmp("0", argv[1]) != 0) || pid < 0){
        printf("kill: not a valid pid\n");
        return;
    }
    int status = _get_proc_status(pid);
    if (status == ERROR){
        printf("kill: process does not exist\n");
        return;
    }
    else{
        _kill(pid);
    }
}