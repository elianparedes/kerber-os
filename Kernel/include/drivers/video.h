#ifndef _VIDEO_
#define _VIDEO_

#include <stdlib.h>
#include <lib.h>
#include <stdint.h>

#define MAX_ROWS 25
#define MAX_COLS 80

enum colors {BLACK = 0,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,LIGHT_GRAY,
DARK_GRAY,LIGHT_BLUE,LIGHT_GREEN,LIGHT_CYAN,LIGHT_RED,LIGHT_MAGENTA,YELLOW,WHITE};

typedef struct area
{
    uint8_t width;
    uint8_t height;
    uint8_t first_col;
    uint8_t first_row;
}area_t;

typedef struct position{
    uint8_t row;
    uint8_t col;
}position_t;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void disable_cursor();

void move_cursor(position_t pos);

position_t get_cursor_position();

void print_string(const char * string);

void print_char(const char c);

void print_new_line();

void delete_char();

void clear_screen();

void clear_line(int line);

void scroll();

void set_working_area(area_t area);

void enable_scroll();

void disable_scroll();

void set_background_color(int first_row , int first_col , int last_row , int last_col, enum colors color);

void set_foreground_color(int first_row , int first_col , int last_row , int last_col, enum colors color);
#endif