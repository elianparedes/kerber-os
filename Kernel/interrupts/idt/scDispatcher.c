#include <interrupts/scDispatcher.h>
#include <interrupts/syscalls.h>
#include <naiveConsole.h>

uint8_t syscall_dispatcher(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t id){
    int syscall_id= (int)id;
    switch(id){
        case SYSCALL_READ:
            return read((int)arg0, (char*)arg1, arg2);
            break;
        case SYSCALL_WRITE:
            return write((int)arg0, (char*)arg1, arg2);
            break;
        case SYSCALL_SWITCH_SCREEN_MODE:
            sys_switch_screen_mode((int)arg0);
            break;
        case SYSCALL_CLEAR_SCREEN:
            sys_clear_screen();
            break;
        case SYSCALL_EXIT:
            sys_exit((int)arg0);
            break;
        case SYSCALL_GETTIME:
            return sys_gettime((time_t *)arg0, (int)arg1);
            break;
        case SYSCALL_RUN:
            sys_run((void *)arg0);
            break;
        default:
            return 0;
    }
    return 0;
}
