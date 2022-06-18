GLOBAL testinforeg

section .text
testinforeg:
mov r15, 15000h
mov r14 , 14000h
mov r13 , 13000h
mov r12 , 12000h
mov r11, 11000h
mov r10, 10000h
mov r9, 9000h
mov r8, 8000h
mov rdx, 7000h
mov rcx, 6000h
mov rbx, 5000h
mov rax, 4000h
mov rsi, 3000h
mov rdi, 2000h
cmp rax, 4000h
je .loop
.loop:
jmp .loop
ret