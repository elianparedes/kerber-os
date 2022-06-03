#include <interrupts/syscalls.h>
#include <interrupts/interrupts.h>
#include <drivers/keyboard.h>
#include <drivers/video.h>

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
    uint16_t i=0;
    while (i < count){
        char c= buffer[i];
        if (c == '\n'){
            print_new_line();
        }
        else{
            print_char(c);
        }
        i++;
    }
    return i;
}