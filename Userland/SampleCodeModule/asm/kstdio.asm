GLOBAL _read
GLOBAL _write
GLOBAL _time

section .text

_read:
    mov rax, SYSCALL_READ_ID
    int 0x80
ret

_write:
    mov rax, SYSCALL_WRITE_ID
    int 0x80
ret

_time:
    mov rax, SYSCALL_GETTIME_ID
    int 0x80
ret

section .rodata
SYSCALL_READ_ID equ 0
SYSCALL_WRITE_ID equ 1
SYSCALL_GETTIME_ID equ 96
