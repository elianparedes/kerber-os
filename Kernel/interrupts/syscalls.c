#include <interrupts/syscalls.h>
#include <interrupts/interrupts.h>
#include <drivers/keyboard.h>
#include <drivers/video.h>

uint8_t read(int fd, char * buffer, uint16_t count){
    //file descriptors not implemented
    if (fd > STDERR){
        return ERROR;
    }
    char last_key=0;
    char key=0;
    uint16_t i=0;
    _sti();
    while (key != 10){
        key=kbd_get_last_key();
        if (key != last_key && i < count){
            buffer[i++]=key;
        }
        last_key=key;

    }
    _cli();
    if (i>0){
        buffer[i-1]=0;
    }
    return i-1;
}

uint8_t write(int fd, char * buffer, uint16_t count){
     //file descriptors not implemented
    if (fd > STDERR){
        return ERROR;
    }
    uint16_t i=0;
    while (buffer[i] != 0 && i < count){
        print_char(buffer[i++]);
    }
    return i;
}