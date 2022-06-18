#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <drivers/video.h>
#include <lib.h>
#include <rtc.h>

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

void gsnapshot();

void gfocus(context_id_t target);

#endif /* _GRAPHICS_H_ */