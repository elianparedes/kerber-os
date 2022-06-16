#include <lib.h>

void kbd_disable();
void kbd_enable();

void kbd_handler();

void kbd_clear_buffer();
char * kbd_get_buffer();
uint16_t kbd_get_current_index();
char kbd_get_last_key();
char kbd_get_rm_last_key();

uint8_t kbd_is_cntrl_pressed();