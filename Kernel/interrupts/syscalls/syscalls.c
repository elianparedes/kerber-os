// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <interrupts.h>
#include <keyboard.h>
#include <registers.h>
#include <scheduler.h>
#include <syscalls.h>
#include <video.h>

#define ADDRESS_LIMIT 0xFFFFFFFF

enum DISTRIBUTION {
    FULL_DISTRIBUTION = 0,
    SPLIT_DISTRIBUTION
};

uint16_t read(int fd, char *buffer, uint16_t count) {
    // file descriptors not implemented
    if (fd > STDERR) {
        return ERROR;
    }
    kbd_clear_buffer();
    _sti();
    while (count > kbd_get_current_index()) {
    }
    _cli();
    char aux[BUFFER_SIZE];
    kbd_get_buffer(aux);
    uint16_t i;
    for (i = 0; i < count; i++) {
        buffer[i] = aux[i];
    }
    return i - 1;
}

uint16_t write(int fd, char *buffer, uint16_t count) {
    // file descriptors not implemented
    if (fd > STDERR) {
        return ERROR;
    }

    process_t *current_process = get_current_process();
    context_id_t gc = current_process->g_context;

    uint16_t i = 0;
    while (i < count) {
        char c = buffer[i];
        if (c == '\n') {
            gprint_new_line(gc);
        } else {
            gprint_char(c, gc);
        }
        i++;
    }
    return i;
}

uint8_t sys_gettime(time_t *struct_time, int utc_offset) {
    // set_UTC_offset(utc_offset);
    get_struct_time();
    return SUCCESS;
}

void sys_exit(int error_code) {
    exit_process();
}

void sys_switch_screen_mode(int mode) {
    if (mode == FULL_DISTRIBUTION) {
        full_screen_distribution();
    }
    if (mode == SPLIT_DISTRIBUTION) {
        split_screen_distribution();
    }
}

void sys_clear_screen() {
    clear_screen();
}

int sys_run(void *main, char *arg) {
    return add_process(main, arg);
}

void sys_delete_char() {
    process_t *current_process = get_current_process();
    context_id_t gc = current_process->g_context;

    gdelete_char(gc);
}

int sys_cntrl_pressed() {
    return kbd_is_cntrl_pressed();
}

int sys_copy_cpu_state(cpu_state_t *cpu_ptr, request_t request) {
    return copy_cpu_state(cpu_ptr, request);
}

uint8_t sys_cntrl_listener(uint8_t *listener) {
    kbd_sets_cntrl_listener(listener);
    return SUCCESS;
}

void sys_kill(int pid) {
    kill_process(pid);
}

void sys_wait(int child, int *pstatus) {
    if (child > 1) {
        *pstatus = -1;
        return;
    }

    process_t *process = get_current_process();
    if (child == 0)
        *pstatus = process->l_child != NULL;
    else if (child == 1)
        *pstatus = process->r_child != NULL;
}

void sys_pause(int pid) {
    process_t *process = get_process(pid);
    if (process != NULL)
        process->status = !process->status;
    return;
}

uint16_t sys_get_mem(uint8_t *address, uint8_t *buffer, uint16_t count) {
    int i;
    for (i = 0; i < count; i++) {
        if ((uint64_t)address > ADDRESS_LIMIT) {
            return i;
        }
        buffer[i] = (*address);
        address++;
    }
    return i;
}

void sys_focus(int pid) {
    process_t *process = get_process(pid);
    if (process != NULL) {
        gfocus(process->g_context);
    }
}

void sys_sched_yield(){
    _force_schedule();
}