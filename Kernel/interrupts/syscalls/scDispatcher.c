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
            return sys_gettime((time_rtc_t *)arg0, (int)arg1);
        case SYSCALL_RUN:
            return sys_run((void *)arg0, (int)arg1, (char **)arg2);
        case SYSCALL_CNTRL_LISTENER:
            return sys_cntrl_listener((uint8_t *)arg0);
        case SYSCALL_DELETE_CHAR:
            sys_delete_char();
            break;
        case SYSCALL_KILL:
            sys_kill((int)arg0);
            break;
        case SYSCALL_BLOCK:
            return sys_block((int)arg0);
            break;
        case SYSCALL_UNBLOCK:
            return sys_unblock((int)arg0);
            break;
        case SYSCALL_GET_MEM:
            return sys_get_mem((uint8_t *)arg0, (uint8_t *)arg1,
                               (uint16_t)arg2);
        case SYSCALL_GET_MEM_STATE:
            sys_get_mem_state((int *)arg0);
            break;
        case SYSCALL_GET_PROC_STATUS:
            return sys_get_proc_status((int)arg0);
            break;
        case SYSCALL_SET_PRIORITY:
            return sys_set_priority((int)arg0, (int)arg1);
            break;
        case SYSCALL_COPY_CPU_STATE:
            return sys_copy_cpu_state((cpu_state_t *)arg0, (request_t)arg1);
        case SYSCALL_FOCUS:
            sys_focus((int)arg0);
            break;
        case SYSCALL_SCHED_YIELD:
            sys_sched_yield();
            break;
        case SYSCALL_WAIT:
            return sys_wait();
        case SYSCALL_SEM_OPEN:
            return sys_sem_open((char *)arg0, (int)arg1);
        case SYSCALL_SEM_WAIT:
            return sys_sem_wait((sem_ptr)arg0);
        case SYSCALL_SEM_POST:
            return sys_sem_post((sem_ptr)arg0);
        case SYSCALL_CLOSE:
            sys_close((int)arg0);
            break;
        case SYSCALL_CREATE_PIPE:
            return sys_create_pipe((char *)arg0, (int *)arg1);
        case SYSCALL_OPEN_PIPE:
            return sys_open_pipe((char *)arg0, (int *)arg1);
        case SYSCALL_SEM_CLOSE:
            return sys_sem_close((sem_ptr)arg0);
        case SYSCALL_INFO_PIPE:
            return sys_info_pipe((char *)arg0, (pipe_info_t *)arg1);
        case SYSCALL_INFO_ALL_PIPES:
            return sys_info_all_pipes((pipe_info_t **)arg0, (unsigned int)arg1);
        case SYSCALL_GET_SEMS:
            return sys_get_semaphores((copy_sem_t **)arg0);
        case SYSCALL_DUP2:
            return sys_dup2((unsigned int)arg0, (unsigned int)arg1);
        case SYSCALL_MALLOC:
            return sys_malloc((size_t)arg0);
        case SYSCALL_FREE:
            sys_free((void *)arg0);
            break;
        case SYSCALL_WAITPID:
            return sys_waitpid((int)arg0, (int *)arg1);
        case SYSCALL_SETFG:
            sys_setfg((int)arg0);
            break;
        case SYSCALL_GET_PROC_TABLE:
            sys_proctable((process_table_t *)arg0);
            break;
        default:
            return 0;
    }
    return 0;
}
