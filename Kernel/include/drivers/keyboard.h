#include <lib.h>
#define BUFFER_SIZE 512
#define KBD_SIZE 128
#define KBD_ENCODER_PORT 0x60
#define KBD_CTRL_PORT 0x64
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

void keyboardInstall();
void keyboardHandler();
void clearBuffer();
char * getBuffer();
uint16_t getCurrentIndex();
char getLastKey();