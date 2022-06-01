GLOBAL getchar
GLOBAL _puts

section .text

getchar:
    mov rax, 0        ;syscall_read
    mov rdi, 0        ;stdin
    mov rsi, buffer   ;buffer
    mov rdx, 1        ;count
    int 0x80
    mov rax, [buffer]
ret

_puts:
    mov rbx, rsi      ;length 
    mov rsi, rdi      ;buffer
    mov rdx, rbx      ;byte count
    mov rax, 1        ;syscall write
    mov rdi, 1        ;stdout
    int 0x80

ret
section .resb

buffer db 30