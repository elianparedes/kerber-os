GLOBAL _read
GLOBAL _write
GLOBAL _time
GLOBAL _run
GLOBAL _wait
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
GLOBAL _wait2
GLOBAL _sem_open
GLOBAL _sem_wait
GLOBAL _sem_post
GLOBAL _sem_close

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

_wait:
    mov rax, SYSCALL_WAIT_ID
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

_wait2:
    mov rax, SYSCALL_WAIT2_ID
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

section .rodata
SYSCALL_READ_ID equ 0
SYSCALL_WRITE_ID equ 1
SYSCALL_SWITCH_SCREEN_MODE_ID equ 4
SYSCALL_CLEAR_SCREEN_ID equ 5
SYSCALL_CNTRL_LISTENER_ID equ 6
SYSCALL_GET_MEM_ID equ 10
SYSCALL_COPY_CPU_STATE equ 7
SYSCALL_EXIT_ID equ 60
SYSCALL_GETTIME_ID equ 96
SYSCALL_RUN_ID equ 66
SYSCALL_WAIT_ID equ 67
SYSCALL_WAIT2_ID equ 68
SYSCALL_DELETE_CHAR_ID equ 46
SYSCALL_KILL_ID equ 62
SYSCALL_PAUSE_ID equ 75 
SYSCALL_FOCUS_ID equ 77   
SYSCALL_SCHED_YIELD_ID equ 24
SYSCALL_SEM_OPEN_ID equ 70
SYSCALL_SEM_WAIT_ID equ 71
SYSCALL_SEM_POST_ID equ 72
SYSCALL_SEM_CLOSE_ID equ 73

KBD_PRINT_REG equ 1