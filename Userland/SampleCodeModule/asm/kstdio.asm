GLOBAL _read
GLOBAL _write
GLOBAL _time
GLOBAL _run
GLOBAL _running
GLOBAL _switch_screen_mode
GLOBAL _clear_screen
GLOBAL _exit
GLOBAL _delete_char
GLOBAL _cntrl_listener
GLOBAL _kill

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

_running:
    mov rax, SYSCALL_RUNNING_ID
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

section .rodata
SYSCALL_READ_ID equ 0
SYSCALL_WRITE_ID equ 1
SYSCALL_SWITCH_SCREEN_MODE_ID equ 4
SYSCALL_CLEAR_SCREEN_ID equ 5
SYSCALL_CNTRL_LISTENER_ID equ 6
SYSCALL_EXIT_ID equ 60
SYSCALL_GETTIME_ID equ 96
SYSCALL_RUN_ID equ 66
SYSCALL_RUNNING_ID equ 67
SYSCALL_DELETE_CHAR_ID equ 46
SYSCALL_KILL_ID equ 62