#include <registers.h>

int snapshot = 0;
cpu_state_t * cpu_state;

void save_cpu_state(cpu_state_t * cpu){
    if(!snapshot)
        snapshot = 1;
    cpu_state = cpu;
}

cpu_state_t * get_cpu_state(){
    return cpu_state;
}

int copy_cpu_state(cpu_state_t* cpu_ptr){
    if(!snapshot)
        return 0;
    cpu_ptr->r15 = cpu_state->r15;
    cpu_ptr->r14 = cpu_state->r14;
    cpu_ptr->r13 = cpu_state->r13;
    cpu_ptr->r12 = cpu_state->r12;
    cpu_ptr->r11 = cpu_state->r11;
    cpu_ptr->r10 = cpu_state->r10;
    cpu_ptr->r9 = cpu_state->r9;
    cpu_ptr->r8 = cpu_state->r8;
    cpu_ptr->rax = cpu_state->rax;
    cpu_ptr->rbp = cpu_state->rbp;
    cpu_ptr->rbx = cpu_state->rbx;
    cpu_ptr->rcx = cpu_state->rcx;
    cpu_ptr->rdi = cpu_state->rdi;
    cpu_ptr->rdx = cpu_state->rdx;
    cpu_ptr->rflags = cpu_state->rflags;
    cpu_ptr->rip = cpu_state->rip;
    cpu_ptr->rsi = cpu_state->rsi;
    cpu_ptr->rsp = cpu_state->rsp;
    return 1;
}