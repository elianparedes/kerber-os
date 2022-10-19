GLOBAL cpuVendor
GLOBAL outb
GLOBAL outw
GLOBAL inb
GLOBAL inw

GLOBAL _rtc_time
GLOBAL _set_hour12_mode
GLOBAL _set_hour24_mode
GLOBAL _set_decimal_mode
GLOBAL _set_BCD_mode

GLOBAL _xadd
GLOBAL _xchg

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

outb:
    mov     rdx, rdi
    mov     rax, rsi
    out     dx, al
    ret

outw:
    mov     rdx, rdi
    mov     rax, rsi
    out     dx, ax
    ret

inb:
    xor     rax, rax
    mov     rdx, rdi
    in      al, dx
    ret

inw:
    xor     rax, rax
    mov     rdx, rdi
    in      ax, dx
    ret

_rtc_time:
	push rbp
	mov rbp , rsp
	mov [aux] , byte rdi
	mov  al , [aux]
	out 70h,al
	in al,71h
	mov rsp, rbp
	pop rbp
ret
	
_set_hour12_mode:
	push rbp
	mov rbp , rsp
	mov al, 11
	out 70h,al
	in al,71h
	mov ah,2
	not ah
	and al,ah
	out 71h,al
	mov rsp, rbp
	pop rbp
ret

_set_hour24_mode:
	push rbp
	mov rbp , rsp
	mov al, 11
	out 70h,al
	in al,71h
	mov ah,2
	or al,ah
	out 71h,al
	mov rsp, rbp
	pop rbp
ret

_set_decimal_mode:
	push rbp
	mov rbp , rsp
	mov al, 11
	out 70h,al
	in al,71h
	mov ah,4
	or al,ah
	out 71h,al
	mov rsp, rbp
	pop rbp
ret

_set_BCD_mode:
	push rbp
	mov rbp , rsp
	mov al, 11
	out 70h,al
	in al,71h
	mov ah,4
	not ah
	and al,ah
	out 71h,al
	mov rsp, rbp
	pop rbp
ret

_xadd:
  mov rax, rsi
  lock xadd [rdi], eax
  ret

_xchg:
  mov rax, rsi
  xchg [rdi], eax
  ret

section .bss
aux resb 1