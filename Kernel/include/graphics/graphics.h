#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lib.h>
#include <rtc.h>
#include <video.h>

typedef enum {
    LEFT = 0,
    RIGHT,
    FULL
} context_id_t;

/**
 * @brief Create a window object
 * 
 * @param area 
 * @return area_t 
 */
area_t create_window(area_t area);

/**
 * @brief 
 * 
 */
void full_screen_distribution();

/**
 * @brief 
 * 
 */
void split_screen_distribution();

/**
 * @brief 
 * 
 * @param c 
 * @param id 
 */
void gprint_char(const char c, context_id_t id);

/**
 * @brief 
 * 
 * @param str 
 * @param id 
 */
void gprint_string(const char *str, context_id_t id);

/**
 * @brief 
 * 
 * @param id 
 */
void gprint_new_line(context_id_t id);

/**
 * @brief 
 * 
 * @param id 
 */
void gdelete_char(context_id_t id);

/**
 * @brief 
 * 
 * @param id 
 */
void gclear_screen(context_id_t id);

/**
 * @brief Get the context id object
 * 
 * @return context_id_t 
 */
context_id_t get_context_id();

/**
 * @brief 
 * 
 */
void gsnapshot();

/**
 * @brief 
 * 
 * @param target 
 */
void gfocus(context_id_t target);

#endif /* _GRAPHICS_H_ */