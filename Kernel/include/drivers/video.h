#ifndef _VIDEO_H
#define _VIDEO_H

#include <lib.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_ROWS 25
#define MAX_COLS 80

/**
 * @brief 16 VGA Colors in 4-bit mode
 * @see https://www.fountainware.com/EXPL/vga_color_palettes.html
 */
enum colors {
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};

typedef struct area {
    uint8_t width;
    uint8_t height;
    uint8_t first_col;
    uint8_t first_row;
} area_t;

typedef struct position {
    uint8_t row;
    uint8_t col;
} position_t;

/**
 * @brief 
 * 
 * @param cursor_start 
 * @param cursor_end 
 */
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

/**
 * @brief 
 * 
 */
void disable_cursor();

/**
 * @brief 
 * 
 * @param pos 
 */
void move_cursor(position_t pos);

/**
 * @brief Get the cursor position object
 * 
 * @return position_t 
 */
position_t get_cursor_position();

/**
 * @brief 
 * 
 * @param string 
 */
void print_string(const char *string);

/**
 * @brief 
 * 
 * @param c 
 */
void print_char(const char c);

/**
 * @brief 
 * 
 */
void print_new_line();

/**
 * @brief 
 * 
 */
void delete_char();

/**
 * @brief 
 * 
 */
void clear_screen();

/**
 * @brief 
 * 
 * @param line 
 */
void clear_line(int line);

/**
 * @brief 
 * 
 */
void scroll();

/**
 * @brief Set the working area object
 * 
 * @param area 
 */
void set_working_area(area_t area);

/**
 * @brief 
 * 
 */
void enable_scroll();

/**
 * @brief 
 * 
 */
void disable_scroll();

/**
 * @brief Set the background color
 * 
 * @param first_row 
 * @param first_col 
 * @param last_row 
 * @param last_col 
 * @param color 
 */
void set_background_color(int first_row, int first_col, int last_row,
                          int last_col, enum colors color);

/**
 * @brief Set the foreground color
 * 
 * @param first_row 
 * @param first_col 
 * @param last_row 
 * @param last_col 
 * @param color 
 */
void set_foreground_color(int first_row, int first_col, int last_row,
                          int last_col, enum colors color);

/**
 * @brief Get the current working area
 * 
 * @return area_t 
 */
area_t get_current_working_area();

#endif