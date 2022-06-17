#include <interrupts/scDispatcher.h>
#include <interrupts/syscalls.h>
#include <naiveConsole.h>

uint8_t syscall_dispatcher(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t id){
    int syscall_id= (int)id;
    switch(id){
        case SYSCALL_READ:
            return read((int)arg0, (char*)arg1, arg2);
        case SYSCALL_WRITE:
            return write((int)arg0, (char*)arg1, arg2);
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
        case SYSCALL_RUN:
            return sys_run((void *)arg0);
        case SYSCALL_CNTRL_LISTENER:
            return sys_cntrl_listener((char *)arg0);
        case SYSCALL_DELETE_CHAR:
            sys_delete_char();
            break;
        case SYSCALL_KILL:
            sys_kill((int)arg0);
            break;
        case SYSCALL_RUNNING:
            return sys_running((int)arg0);
        default:
            return 0;
    }
    return 0;
}
