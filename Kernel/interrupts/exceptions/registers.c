#include <registers.h>

int snapshot = 0;
cpu_state_t cpu_state_kbd;
cpu_state_t cpu_state_exception;

static void copy(cpu_state_t *from, cpu_state_t *dest) {
    dest->r15 = from->r15;
    dest->r14 = from->r14;
    dest->r13 = from->r13;
    dest->r12 = from->r12;
    dest->r11 = from->r11;
    dest->r10 = from->r10;
    dest->r9 = from->r9;
    dest->r8 = from->r8;
    dest->rax = from->rax;
    dest->rbp = from->rbp;
    dest->rbx = from->rbx;
    dest->rcx = from->rcx;
    dest->rdi = from->rdi;
    dest->rdx = from->rdx;
    dest->rflags = from->rflags;
    dest->rip = from->rip;
    dest->rsi = from->rsi;
    dest->rsp = from->rsp;
}

void save_cpu_state(cpu_state_t *cpu, request_t request) {
    if (request == EXCEPTION) {
        copy(cpu, &cpu_state_exception);
    } else {
        if (!snapshot)
            snapshot = 1;
        copy(cpu, &cpu_state_kbd);
    }
}

int copy_cpu_state(cpu_state_t *cpu_ptr, request_t request) {
    if (request == EXCEPTION) {
        cpu_ptr->r15 = cpu_state_exception.r15;
        cpu_ptr->r14 = cpu_state_exception.r14;
        cpu_ptr->r13 = cpu_state_exception.r13;
        cpu_ptr->r12 = cpu_state_exception.r12;
        cpu_ptr->r11 = cpu_state_exception.r11;
        cpu_ptr->r10 = cpu_state_exception.r10;
        cpu_ptr->r9 = cpu_state_exception.r9;
        cpu_ptr->r8 = cpu_state_exception.r8;
        cpu_ptr->rax = cpu_state_exception.rax;
        cpu_ptr->rbp = cpu_state_exception.rbp;
        cpu_ptr->rbx = cpu_state_exception.rbx;
        cpu_ptr->rcx = cpu_state_exception.rcx;
        cpu_ptr->rdi = cpu_state_exception.rdi;
        cpu_ptr->rdx = cpu_state_exception.rdx;
        cpu_ptr->rflags = cpu_state_exception.rflags;
        cpu_ptr->rip = cpu_state_exception.rip;
        cpu_ptr->rsi = cpu_state_exception.rsi;
        cpu_ptr->rsp = cpu_state_exception.rsp;
        return 1;
    } else {
        if (!snapshot)
            return 0;
        cpu_ptr->r15 = cpu_state_kbd.r15;
        cpu_ptr->r14 = cpu_state_kbd.r14;
        cpu_ptr->r13 = cpu_state_kbd.r13;
        cpu_ptr->r12 = cpu_state_kbd.r12;
        cpu_ptr->r11 = cpu_state_kbd.r11;
        cpu_ptr->r10 = cpu_state_kbd.r10;
        cpu_ptr->r9 = cpu_state_kbd.r9;
        cpu_ptr->r8 = cpu_state_kbd.r8;
        cpu_ptr->rax = cpu_state_kbd.rax;
        cpu_ptr->rbp = cpu_state_kbd.rbp;
        cpu_ptr->rbx = cpu_state_kbd.rbx;
        cpu_ptr->rcx = cpu_state_kbd.rcx;
        cpu_ptr->rdi = cpu_state_kbd.rdi;
        cpu_ptr->rdx = cpu_state_kbd.rdx;
        cpu_ptr->rflags = cpu_state_kbd.rflags;
        cpu_ptr->rip = cpu_state_kbd.rip;
        cpu_ptr->rsi = cpu_state_kbd.rsi;
        cpu_ptr->rsp = cpu_state_kbd.rsp;
        return 1;
    }
}