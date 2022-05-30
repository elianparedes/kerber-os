#include <drivers/keyboard.h>
#include <naiveConsole.h>

#define KBD_ENCODER_PORT 0x60
#define KBD_CTRL_PORT 0x64
/// 0x64 mask status register
enum KBD_CTRL_MASK {
 
	KBD_CTRL_MASK_OUT_BUF	=	1,		//00000001
	KBD_CTRL_MASK_IN_BUF	=	2,		//00000010
	KBD_CTRL_MASK_SYSTEM	=	4,		//00000100
	KBD_CTRL_MASK_CMD_DATA	=	8,		//00001000
	KBD_CTRL_MASK_LOCKED	=	0x10,		//00010000
	KBD_CTRL_MASK_AUX_BUF	=	0x20,		//00100000
	KBD_CTRL_MASK_TIMEOUT	=	0x40,		//01000000
	KBD_CTRL_MASK_PARITY	=	0x80		//10000000
};
enum KBD_CTRL_CMD{
  KBD_CTRL_CMD_DISABLE = 0xAD, 
  KBD_CTRL_CMD_ENABLE = 0xAE
};
/*
enum KBD_ENC_CMD{
  KBD_ENC_CMD_SETSC= 0xF0
};
enum KBD_SC{
  KBD_SC_1=0x01,
  KBD_SC_2=0x02,
  KBD_SC_3=0x03
};
*/

#define BUFFER_SIZE 512
#define KBD_SIZE 128
#define MAYUS_OFFSET ('a'-'A')
#define IS_ASCII_LETTER(l) (l >= 'a' && l <= 'z')

static char buffer[BUFFER_SIZE];
static uint16_t index=0;
static uint8_t caps_locked=0;

static char kbd_US_1 [KBD_SIZE] =
{
    0,  27 /* Esc */, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 
    '\b' /* Backspace */, '\t', /* Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
    '\n' /* Enter */, 0, /* control key */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
   '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* Undefined Keys*/
};


void kbd_install(){
    set_kbd_handler(kbd_handler);
}

void kbd_send_enc_cmd(uint8_t cmd){
	while (1){
		if ( (inb(KBD_CTRL_PORT) & KBD_CTRL_MASK_IN_BUF) == 0)
			break;
    }
    outb(KBD_ENCODER_PORT, cmd);
}

void kbd_send_ctrl_cmd(uint8_t cmd){
    while (1){
		if ( (inb(KBD_CTRL_PORT) & KBD_CTRL_MASK_IN_BUF) == 0)
			break;
    }
    outb(KBD_CTRL_PORT, cmd);
}

void kbd_disable(){
    kbd_send_ctrl_cmd(KBD_CTRL_CMD_DISABLE);
}

void kbd_enable(){
    kbd_send_ctrl_cmd(KBD_CTRL_CMD_ENABLE);
}

void kbd_handler(){

    uint8_t scan_code= inb(KBD_ENCODER_PORT);
    if (index > BUFFER_SIZE || scan_code > KBD_SIZE){
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

char * kbd_get_buffer(){
    char toReturn[BUFFER_SIZE];
    memcpy(toReturn, buffer, index);
    return toReturn;
}

void kbd_clear_buffer(){
    buffer[0]=0;
    index=0;
}

uint16_t kbd_get_current_index(){
    return index;
}

char kbd_get_last_key(){
    if (index > 0){
        return buffer[index-1];
    }
    return 0;
}