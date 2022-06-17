#ifndef _GRAPHICS_
#define _GRAPHICS_

//     0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
// B			    │	┤	╡	╢	╖	╕	╣	║	╗	╝	╜	╛	┐
// C   └	┴	┬	├	─	┼	╞	╟	╚	╔	╩	╦	╠	═	╬	╧
// D   ╨	╤	╥	╙	╘	╒	╓	╫	╪	┘	┌					

#define LEFT_TOP_CORNER 0xDA
#define RIGHT_TOP_CORNER 0xBF
#define LEFT_BOTTOM_CORNER 0xC0
#define RIGHT_BOTTOM_CORNER 0XD9
#define HZ_LINE 0xC4
#define VT_LINE 0xB3
#define MIDDLE_TOP 0xC2
#define MIDDLE_BOTTOM 0xC1

#include <graphics.h>

typedef struct gcontext{
    area_t area;
    position_t last_cursor_pos;
} gcontext_t;

typedef enum {FULL_DISTRIBUTION=0,SPLIT_DISTRIBUTION} distribution_t;

static int allocated_windows_count = 0; 
distribution_t current_distribution = FULL_DISTRIBUTION;

gcontext_t contexts[3];

static void draw_top_line(area_t area){
    int line= area.first_row;
    int first_col = area.first_col;
    move_cursor((position_t){line,first_col});
    print_char(LEFT_TOP_CORNER);
    for(int i = area.first_col+1 ; i < (area.first_col + area.width-1) ; i++){
        print_char(HZ_LINE);
    }
    print_char(RIGHT_TOP_CORNER);
}

static void draw_bottom_line(area_t area){
    int line= area.first_row+area.height-1;
    int first_col = area.first_col;
    move_cursor((position_t){line,first_col});
    print_char(LEFT_BOTTOM_CORNER);
    for(int i = first_col+1; i < (area.first_col + area.width-1) ; i++){
        print_char(HZ_LINE);
    }
    print_char(RIGHT_BOTTOM_CORNER);
}

area_t create_window(area_t area){
    enable_cursor(15,16);
    disable_scroll();

    move_cursor((position_t){area.first_row,area.first_col});
    set_working_area(area);
    //set_background_color(area.first_row,area.first_col,area.first_row+area.height-1,area.first_col+area.width-1,LIGHT_GRAY);
    set_foreground_color(area.first_row,area.first_col,area.first_row+area.height-1,area.first_col+area.width-1,LIGHT_BLUE);
    draw_bottom_line(area);
    draw_top_line(area);
    for(int i = area.first_row +1 ; i < (area.first_row + area.height -1) ; i++){
        for(int j = 0 ; j < 2 ; j++){
            if(j%2){
                move_cursor((position_t){i,area.first_col+area.width-1});
                print_char(VT_LINE);
            }
            else{
                move_cursor((position_t){i,area.first_col});
                print_char(VT_LINE);
            }
        }
    }
    area_t new_area = {area.width-2,area.height-2 , area.first_col+1,area.first_row+1};
    set_working_area(new_area);
    set_foreground_color(new_area.first_row,new_area.first_col,new_area.first_row+new_area.height-1,new_area.first_col+new_area.width-1,WHITE);
    move_cursor((position_t){new_area.first_row,new_area.first_col});
    enable_scroll();
    return new_area;
}

void create_line(area_t area , enum colors color){
    if(area.height > 1)
        return;
    enable_cursor(15,16);
    move_cursor((position_t){area.first_row,area.first_col});

    set_working_area(area);
    set_background_color(area.first_row,area.first_col,area.first_row+area.height-1,area.first_col+area.width-1,color);
}

/**
 * COLECCTION[cursor_pos,cursor_pos]
 * count  = 0
 * count = 1
 * 
 * print_c(id,c)
 * 
 * 
 */

context_id_t get_context_id(){
    switch (current_distribution)
    {
        case FULL_DISTRIBUTION:
            return FULL;

        case SPLIT_DISTRIBUTION:
            if(allocated_windows_count == 0){
                allocated_windows_count++;
                return LEFT;
            }
            else
                return RIGHT;
    }
}

void split_screen_distribution(){

    allocated_windows_count = 0;
    current_distribution = SPLIT_DISTRIBUTION;

    area_t screen = {MAX_COLS,MAX_ROWS-2,0,1};

    set_working_area(screen);
    create_window(screen);
    clear_screen();

    set_foreground_color(1,MAX_COLS/2,MAX_ROWS-2,MAX_COLS/2,LIGHT_BLUE);
    move_cursor((position_t){1, MAX_COLS/2});
    print_char(MIDDLE_TOP);
    for(int i = 2 ; i < MAX_ROWS-2;i++){
        move_cursor((position_t){i,MAX_COLS/2});
        print_char(VT_LINE);
    }
    move_cursor((position_t){MAX_ROWS-2, MAX_COLS/2});
    print_char(MIDDLE_BOTTOM);

    
    area_t left = {MAX_COLS/2-2,MAX_ROWS-4,1,2};
    contexts[0].area = left;
    move_cursor((position_t){left.first_row,left.first_col});
    contexts[0].last_cursor_pos = get_cursor_position();

    area_t right = {MAX_COLS/2-2,MAX_ROWS-4,MAX_COLS/2+1,2};
    contexts[1].area = right;
    move_cursor((position_t){right.first_row,right.first_col});
    contexts[1].last_cursor_pos = get_cursor_position();

    area_t bottom_line = {MAX_COLS,1,0,MAX_ROWS-1};
    create_line(bottom_line,LIGHT_BLUE);

    /*area_t screen = {MAX_COLS,MAX_ROWS-2,0,1};
	set_working_area(screen);
    clear_screen();

    area_t left = {MAX_COLS/2,MAX_ROWS-2,0,1};
	left = create_window(left);
    contexts[0].area = left;
    contexts[0].last_cursor_pos = get_cursor_position();

    area_t right = {MAX_COLS/2,MAX_ROWS-2,MAX_COLS/2,1};
    right = create_window(right);
    contexts[1].area = right;
    contexts[1].last_cursor_pos = get_cursor_position();

    area_t top_line = {MAX_COLS,1,0,0};
    create_line(top_line,LIGHT_CYAN);
    area_t bottom_line = {MAX_COLS,1,0,MAX_ROWS-1};
    create_line(bottom_line,LIGHT_BLUE);*/
}

void full_screen_distribution(){
    allocated_windows_count = 0;
    current_distribution = FULL;

    area_t screen = {MAX_COLS,MAX_ROWS-2,0,1};
	screen = create_window(screen);
    contexts[2].area = screen;
    contexts[2].last_cursor_pos = get_cursor_position();

    clear_screen();

    area_t top_line = {MAX_COLS,1,0,0};
    create_line(top_line,DARK_GRAY);
    area_t bottom_line = {MAX_COLS,1,0,MAX_ROWS-1};
    create_line(bottom_line,LIGHT_BLUE);
}

static void load_context(context_id_t id){
    set_working_area(contexts[id].area);
    move_cursor(contexts[id].last_cursor_pos);
}

void gprint_char(const char c , context_id_t id){
    load_context(id);
    print_char(c);
    contexts[id].last_cursor_pos=get_cursor_position();
}

void gprint_string(const char * str ,context_id_t id){
    load_context(id);
    print_string(str);
    contexts[id].last_cursor_pos=get_cursor_position();
}

void gprint_new_line(context_id_t id){
    load_context(id);
    print_new_line();
    contexts[id].last_cursor_pos=get_cursor_position();
}

void gdelete_char(context_id_t id){
    load_context(id);
    delete_char();
    contexts[id].last_cursor_pos=get_cursor_position();
}

void gclear_screen(context_id_t id){
    load_context(id);
    clear_screen();
    contexts[id].last_cursor_pos=get_cursor_position();
}

void snapshot_animation(){
    area_t current = get_current_working_area();
    set_working_area((area_t){MAX_COLS,MAX_ROWS,0,0});
    uint64_t seconds = get_seconds();
    set_background_color(1,0,MAX_ROWS-2,MAX_COLS-1,LIGHT_GRAY);
    while(get_seconds() - seconds < 1);
    set_background_color(1,0,MAX_ROWS-2,MAX_COLS-1, BLACK);
    set_working_area(current);
}

#endif