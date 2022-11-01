// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>
#include <scDispatcher.h>
#include <syscalls.h>

uint8_t syscall_dispatcher(uint64_t arg0, uint64_t arg1, uint64_t arg2,
                           uint64_t arg3, uint64_t arg4, uint64_t arg5,
                           uint64_t id) {
    switch (id) {
        case SYSCALL_READ:
            return read((int)arg0, (char *)arg1, arg2);
        case SYSCALL_WRITE:
            return write((int)arg0, (char *)arg1, arg2);
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
            return sys_run((void *)arg0, (char *)arg1);
        case SYSCALL_CNTRL_LISTENER:
            return sys_cntrl_listener((uint8_t *)arg0);
        case SYSCALL_DELETE_CHAR:
            sys_delete_char();
            break;
        case SYSCALL_KILL:
            sys_kill((int)arg0);
            break;
        case SYSCALL_PAUSE:
            sys_pause((int)arg0);
            break;
        case SYSCALL_WAIT:
            sys_wait((int)arg0, (int *)arg1);
            break;
        case SYSCALL_GET_MEM:
            return sys_get_mem((uint8_t *)arg0, (uint8_t *)arg1,
                               (uint16_t)arg2);
        case SYSCALL_COPY_CPU_STATE:
            return sys_copy_cpu_state((cpu_state_t *)arg0, (request_t)arg1);
        case SYSCALL_FOCUS:
            sys_focus((int)arg0);
            break;
        case SYSCALL_SCHED_YIELD:
            sys_sched_yield();
            break;
        case SYSCALL_WAIT2:
            sys_wait2();
            break;
        case SYSCALL_SEM_OPEN:
            return sys_sem_open((char *)arg0, (int)arg1);
            break;
        case SYSCALL_SEM_WAIT:
            return sys_sem_wait((sem_ptr)arg0);
            break;
        case SYSCALL_SEM_POST:
            return sys_sem_post((sem_ptr)arg0);
            break;
        case SYSCALL_CLOSE:
            sys_close((int)arg0);
            break;
        case SYSCALL_CREATE_PIPE:
            return sys_create_pipe((char *)arg0, (int *)arg1);
        case SYSCALL_OPEN_PIPE:
            return sys_open_pipe((char *)arg0, (int *)arg1);
        case SYSCALL_SEM_CLOSE:
            return sys_sem_close((sem_ptr)arg0);
            break;
        case SYSCALL_GET_SEMS:
            return sys_get_semaphores((copy_sem_t **)arg0);
            break;
        case SYSCALL_MALLOC:
            return sys_malloc((size_t)arg0);
            break;
        case SYSCALL_FREE:
            sys_free((void *)arg0);
            break;
        default:
            return 0;
    }
    return 0;
}
