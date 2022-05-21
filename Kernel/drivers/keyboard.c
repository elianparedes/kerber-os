#include <drivers/keyboard.h>

void keyboardHandler(){

    uint8_t scan_code= inb(KBD_ENCODER_PORT);
    if (scan_code > KBD_SIZE){
        return; 
    }
    if (scan_code == 0x3A){
        caps_locked=!caps_locked;
        return;
    }
    uint8_t character=kbd_US_1[scan_code];
    if (caps_locked && IS_ASCII_LETTER(character)){
        buffer[index]=character - MAYUS_OFFSET;
    }
    else{
        buffer[index]=character;
    }
    index++;
    buffer[index]=0;
}

char * getBuffer(){
    char toReturn[BUFFER_SIZE];
    memcpy(toReturn, buffer, index);
    return toReturn;
}

void clearBuffer(){
    buffer[0]=0;
    index=0;
}

uint16_t getCurrentIndex(){
    return index;
}

char getLastKey(){
    if (index > 0){
        return buffer[index-1];
    }
    return '\0';
}