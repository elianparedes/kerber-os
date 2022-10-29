GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL _syscall_master_handler
GLOBAL _force_schedule

GLOBAL _force_timer_int

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN schedule
EXTERN save_cpu_state
EXTERN syscall_dispatcher
EXTERN kbd_is_save_reg_shortcut
EXTERN gsnapshot

section .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro saveRegisters 0
	pushState	
	mov rbx , rsp
	add rbx , 120
	; RIP
	push QWORD [rbx]
	add rbx, 16
	; RFLAGS
	push QWORD [rbx]
	add rbx, 8
	; RSP
	push QWORD [rbx]
	mov rdi, rsp
%endmacro

%macro irqHandlerMaster 1
	pushState
	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	mov rdi, %1
	cmp rdi, 0
	je .schedule
	cmp rdi, 1
	jne .EOI

	call kbd_is_save_reg_shortcut
	cmp rax,1
	jne .EOI
	popState
	saveRegisters
	mov rsi,KBD
	call save_cpu_state
	call gsnapshot
	add rsp,24
	jmp .EOI

.schedule:
	mov rdi, rsp
	call schedule
	mov rsp, rax

.EOI:
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
	popState
	iretq
%endmacro

%macro exceptionHandler 1
;https://os.phil-opp.com/cpu-exceptions/#behind-the-scenes
	saveRegisters
	mov rsi,EXCEPTION
	call save_cpu_state
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

_force_timer_int:
	int 20h
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out		0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
irqHandlerMaster 4

;USB
_irq05Handler:
irqHandlerMaster 5

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

_force_schedule:
	int 20h
	ret

_syscall_master_handler:
	cli

	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov rcx, r10

	push rax
	call syscall_dispatcher 
	pop rbx

	mov rbx,rax
	mov al, 20h
	out 20h, al

	mov rax, rbx   ;restore return

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx

	iretq

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1

SECTION .rodata
	EXCEPTION equ 0
	KBD equ 1