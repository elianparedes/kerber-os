#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <drivers/video.h>
typedef enum {LEFT=0,RIGHT,FULL} context_id_t;

area_t create_window(area_t area);

void full_screen_distribution();

void split_screen_distribution();

void gprint_char(const char c , context_id_t id);

void gprint_string(const char * str ,context_id_t id);

void gprint_new_line(context_id_t id);

void gdelete_char(context_id_t id);

void gclear_screen(context_id_t id);

context_id_t get_context_id();

#endif