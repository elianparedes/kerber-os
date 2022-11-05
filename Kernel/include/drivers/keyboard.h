#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define BUFFER_SIZE 512

#include <lib.h>

/**
 * @brief 
 * 
 */
void kbd_disable();

/**
 * @brief 
 * 
 */
void kbd_enable();

/**
 * @brief 
 * 
 */
void kbd_handler();

/**
 * @brief 
 * 
 */
void kbd_clear_buffer();

/**
 * @brief 
 * 
 * @return char* 
 */
int kbd_get_buffer(char * buffer_ret);

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t kbd_get_current_index();

/**
 * @brief 
 * 
 * @return char 
 */
char kbd_get_last_key();

/**
 * @brief 
 * 
 * @return char 
 */
char kbd_get_rm_last_key();

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t kbd_is_cntrl_pressed();

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t kbd_is_save_reg_shortcut();

/**
 * @brief 
 * 
 * @param listener 
 */
void kbd_sets_cntrl_listener(uint8_t *listener);

#endif