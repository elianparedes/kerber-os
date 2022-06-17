#include <interrupts/syscalls.h>
#include <interrupts/interrupts.h>
#include <drivers/keyboard.h>
#include <drivers/video.h>
#include <process/scheduler.h>
#include <registers.h>

enum DISTRIBUTION {FULL_DISTRIBUTION=0, SPLIT_DISTRIBUTION};

uint16_t read(int fd, char * buffer, uint16_t count){
    //file descriptors not implemented
    if (fd > STDERR){
        return ERROR;
    }
    kbd_clear_buffer();
    _sti();
    while (count > kbd_get_current_index()){

    }
    _cli();
    char * aux=kbd_get_buffer();
    uint16_t i;
    for (i=0; i < count ; i++){
        buffer[i]=aux[i];
    }
    return i-1;
}

uint16_t write(int fd, char * buffer, uint16_t count){
     //file descriptors not implemented
    if (fd > STDERR){
        return ERROR;
    }

    process_t * current_process = get_current_process();
    context_id_t gc = current_process->g_context;

    uint16_t i=0;
    while (i < count){
        char c= buffer[i];
        if (c == '\n'){
            gprint_new_line(gc);
        }
        else{
            gprint_char(c, gc);
        }
        i++;
    }
    return i;
}

uint8_t sys_gettime(time_t * struct_time, int utc_offset){
    //set_UTC_offset(utc_offset);
    get_struct_time();
    return 1;
}

void sys_exit(int error_code){
    exit_process();
}

void sys_switch_screen_mode(int mode){
    if (mode == FULL_DISTRIBUTION){
        full_screen_distribution();
    }
    if (mode == SPLIT_DISTRIBUTION){
        split_screen_distribution();
    }
}

void sys_clear_screen(){
    clear_screen();
}

void sys_run(void *main){
    add_process(main);
}

void sys_delete_char(){
    process_t * current_process = get_current_process();
    context_id_t gc = current_process->g_context;

    gdelete_char(gc);
}

int sys_cntrl_pressed(){
    return kbd_is_cntrl_pressed();
}

int sys_copy_cpu_state(cpu_state_t* cpu_ptr){
    return  copy_cpu_state(cpu_ptr);
}

uint8_t sys_cntrl_listener(char * listener){
    kbd_sets_cntrl_listener(listener);
    return SUCCESS;

}