GLOBAL _read
GLOBAL _write
GLOBAL _close
GLOBAL _time
GLOBAL _run
GLOBAL _switch_screen_mode
GLOBAL _clear_screen
GLOBAL _exit
GLOBAL _copy_cpu_state
GLOBAL _delete_char
GLOBAL _cntrl_listener
GLOBAL _kill
GLOBAL _get_mem
GLOBAL _pause
GLOBAL _focus
GLOBAL _sched_yield
GLOBAL _wait
GLOBAL _waitpid
GLOBAL _sem_open
GLOBAL _sem_wait
GLOBAL _sem_post
GLOBAL _sem_close
GLOBAL _create_pipe
GLOBAL _open_pipe
GLOBAL _info_pipe
GLOBAL _info_all_pipes
GLOBAL _dup2
GLOBAL _get_semaphores
GLOBAL _malloc
GLOBAL _free
GLOBAL _get_mem_state
GLOBAL _setfg

section .text

_read:
    mov rax, SYSCALL_READ_ID
    int 0x80
    ret

_write:
    mov rax, SYSCALL_WRITE_ID
    int 0x80
    ret

_switch_screen_mode:
    mov rax, SYSCALL_SWITCH_SCREEN_MODE_ID
    int 0x80
    ret

_clear_screen:
    mov rax, SYSCALL_CLEAR_SCREEN_ID
    int 0x80
    ret

_exit:
    mov rax, SYSCALL_EXIT_ID
    int 0x80
    ret

_time:
    mov rax, SYSCALL_GETTIME_ID
    int 0x80
    ret

_run:
    mov rax, SYSCALL_RUN_ID
    int 0x80
    ret

_cntrl_listener:
    mov rax, SYSCALL_CNTRL_LISTENER_ID
    int 0x80
    ret

_delete_char:
    mov rax, SYSCALL_DELETE_CHAR_ID
    int 0x80
    ret

_kill:
    mov rax, SYSCALL_KILL_ID
    int 0x80
    ret
    
_get_mem:
    mov rax, SYSCALL_GET_MEM_ID
    int 0x80
    ret

_copy_cpu_state:
    mov rax, SYSCALL_COPY_CPU_STATE
    int 0x80
    ret

_pause:
    mov rax, SYSCALL_PAUSE_ID
    int 0x80
    ret

_focus:
    mov rax, SYSCALL_FOCUS_ID
    int 0x80
    ret

_sched_yield:
    mov rax, SYSCALL_SCHED_YIELD_ID
    int 0x80
    ret

_wait:
    mov rax, SYSCALL_WAIT_ID
    int 0x80
    ret

_waitpid:
    mov rax, SYSCALL_WAITPID_ID
    int 0x80
    ret

_sem_open:
    mov rax, SYSCALL_SEM_OPEN_ID
    int 0x80
    ret

_sem_wait:
    mov rax, SYSCALL_SEM_WAIT_ID
    int 0x80
    ret

_sem_post:
    mov rax, SYSCALL_SEM_POST_ID
    int 0x80
    ret

_sem_close:
    mov rax, SYSCALL_SEM_CLOSE_ID
    int 0x80
    ret

_close:
    mov rax, SYSCALL_CLOSE
    int 0x80
    ret

_create_pipe:
    mov rax, SYSCALL_CREATE_PIPE
    int 0x80
    ret

_open_pipe:
    mov rax, SYSCALL_OPEN_PIPE
    int 0x80
    ret

_info_pipe:
    mov rax, SYSCALL_INFO_PIPE
    int 0x80
    ret

_info_all_pipes:
    mov rax, SYSCALL_INFO_ALL_PIPES
    int 0x80
    ret

_dup2:
    mov rax, SYSCALL_DUP2
    int 0x80
    ret

_get_semaphores:
    mov rax, SYSCALL_GET_SEMS_ID
    int 0x80
    ret

_malloc:
    mov rax, SYSCALL_MALLOC_ID
    int 0x80
    ret

_free:
    mov rax, SYSCALL_FREE_ID
    int 0x80
    ret

_get_mem_state:
    mov rax, SYSCALL_GET_MEM_STATE_ID
    int 0x80
    ret

_setfg:
    mov rax, SYSCALL_SET_FG_ID
    int 0x80
    ret

section .rodata
SYSCALL_READ_ID equ 0
SYSCALL_WRITE_ID equ 1
SYSCALL_SWITCH_SCREEN_MODE_ID equ 4
SYSCALL_CLEAR_SCREEN_ID equ 5
SYSCALL_CNTRL_LISTENER_ID equ 6
SYSCALL_MALLOC_ID equ 9
SYSCALL_GET_MEM_ID equ 10
SYSCALL_FREE_ID equ 11
SYSCALL_COPY_CPU_STATE equ 7
SYSCALL_EXIT_ID equ 60
SYSCALL_GETTIME_ID equ 96
SYSCALL_RUN_ID equ 66
SYSCALL_WAIT_ID equ 68
SYSCALL_WAITPID_ID equ 69
SYSCALL_DELETE_CHAR_ID equ 46
SYSCALL_KILL_ID equ 62
SYSCALL_PAUSE_ID equ 75 
SYSCALL_FOCUS_ID equ 77   
SYSCALL_SCHED_YIELD_ID equ 24
SYSCALL_SEM_OPEN_ID equ 70
SYSCALL_SEM_WAIT_ID equ 71
SYSCALL_SEM_POST_ID equ 72
SYSCALL_SEM_CLOSE_ID equ 73
SYSCALL_CLOSE equ 50
SYSCALL_CREATE_PIPE equ 51
SYSCALL_OPEN_PIPE equ 52
SYSCALL_INFO_PIPE equ 53
SYSCALL_INFO_ALL_PIPES equ 54
SYSCALL_DUP2 equ 55
SYSCALL_GET_SEMS_ID equ 74
SYSCALL_GET_MEM_STATE_ID equ 90
SYSCALL_SET_FG_ID equ 92

KBD_PRINT_REG equ 1