#include <stdint.h>
#include <lib.h>
#include <drivers/video.h>

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define CRTC_ADRESS_PORT 0x3D4
#define CRTC_DATA_PORT 0x3D5

#define CURSOR_START_REGISTER 0x0A
#define CURSOR_END_REGISTER 0x0B

#define CURSOR_LOCATION_HIGH_REGISTER 0x0E
#define CURSOR_LOCATION_LOW_REGISTER 0x0F

#define CURSOR_MAX_HEIGHT 15
#define VIDEO_OFFSET() (current_video - video)
#define CURRENT_COL() (VIDEO_OFFSET()%(2*MAX_COLS))
#define CURRENT_LINE() (VIDEO_OFFSET()/(2*MAX_COLS))
#define LINE_ADDRESS(LINE)((LINE)*2*MAX_COLS + video)

static uint8_t * video = (uint8_t *) VIDEO_ADDRESS;
static uint8_t * current_video = (uint8_t *) VIDEO_ADDRESS;
static uint8_t width = MAX_COLS;
static uint8_t height = MAX_ROWS;
static uint8_t first_col = 0; 

static int cursor_row = 0;
static int cursor_col = 0;
static int cursor_enable = 0;

/**
 * TODO: Incluir este link que es de donde "copiamos" el codigo
 * https://wiki.osdev.org/Text_Mode_Cursor#Moving_the_Cursor_2
 **/

static void update_current_video(){
    current_video = video + cursor_row*2*MAX_COLS + cursor_col*2 + first_col*2;
}

static void update_cursor_position(){

    if(!cursor_enable)
        return;

    cursor_col = CURRENT_COL()/2;
    cursor_row = CURRENT_LINE();
    uint16_t pos = cursor_row* MAX_COLS + cursor_col;

    outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_LOW_REGISTER);
	outb(CRTC_DATA_PORT, (uint8_t) pos );
	outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_HIGH_REGISTER);
	outb(CRTC_DATA_PORT, (uint8_t)(pos >> 8));

}

static void copy_line(int line) {

    uint8_t * aux_video_ptr = LINE_ADDRESS(line);

    for(int j = first_col ; j < width ; j++){
        aux_video_ptr[j + MAX_COLS*2] = j%2 ? 
            ((aux_video_ptr[j] & 0x0F) | (aux_video_ptr[j + MAX_COLS*2] & 0xF0)) 
            : aux_video_ptr[j]  ;
    }
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end){
	outb(CRTC_ADRESS_PORT, CURSOR_START_REGISTER);
	outb(CRTC_DATA_PORT, (inb(CRTC_DATA_PORT) & 0xC0) | cursor_start);

	outb(CRTC_ADRESS_PORT, CURSOR_END_REGISTER);
	outb(CRTC_DATA_PORT, (inb(CRTC_DATA_PORT) & 0xA0) | cursor_end );
    cursor_enable = 1;
    update_cursor_position();
}

void disable_cursor(){
	outb(CRTC_ADRESS_PORT, CURSOR_START_REGISTER);
	outb(CRTC_DATA_PORT, 0x20);
    cursor_enable = 0;
}

void move_cursor(int row, int col){
    if(!cursor_enable)
        return;
    cursor_col = col;
    cursor_row = row;
    update_current_video();
	update_cursor_position();
}

void get_cursor_position(int pos[2]){
    pos[0] = cursor_row;
    pos[1] = cursor_col;
}

void print_string(const char * string){
    for(int i = 0 ; string[i] != 0 ; i++){
        print_char(string[i]);
    }
}

void print_char(const char c){

    int current_col = CURRENT_COL();
    int current_line = CURRENT_LINE();
    *current_video = c;

    int is_last_col = (current_col == 2*(width+first_col-1));
    int is_last_row = (current_line == (height-1));

    if( is_last_col && !is_last_row ){
        print_new_line();
        return;
    }

    if( is_last_col && is_last_row ){
        scroll();
        return;
    }

    current_video+=2;
    update_cursor_position();
}

void print_new_line(){

    int current_col = CURRENT_COL();
    int current_line = CURRENT_LINE();
    
    if(current_line == height-1)
        scroll();
    else
        current_line++;
    current_video = LINE_ADDRESS(current_line)+2*first_col;
    update_cursor_position();

}

void delete_char(){

    if(!cursor_enable)
        return;

    int current_line = CURRENT_LINE();
    int current_col = CURRENT_COL();

    current_video -=2;
    current_video[0] = 0;

    if(current_col == first_col){
        if(current_line != 0)
            current_video = LINE_ADDRESS(current_line-1) + 2*(width+first_col-((width==MAX_COLS)? 1:0));
        else
            return;
    }
    update_cursor_position();
}

void clear_screen(){
    for(int i = 0 ; i < MAX_ROWS ; i++){
        clear_line(i);
    }
    current_video = video;
}

void clear_line(int line){
    uint8_t * current_line_video  = LINE_ADDRESS(line) +2*first_col;
    for(int i  =0 ; i < width*2 ; i++){
        if(!(i%2))
            current_line_video[i] = 0 ;
    }
}

void scroll(){
    for(int i = 1 ; i < MAX_ROWS ; i++){
        uint8_t * current_line_video = LINE_ADDRESS(i)+2*first_col;
        for(int j = 0 ; j < width*2 ; j++){
            current_line_video[j-2*MAX_COLS] = current_line_video[j];
        }
    }
    clear_line(MAX_ROWS-1);
    current_video = LINE_ADDRESS(MAX_ROWS-1)+2*first_col;
    update_cursor_position();
}

void set_working_area(int fst_col , int a_width){
    width = a_width;
    first_col = fst_col;
    current_video = video + 2*first_col;
}

void full_screen(){
    width = MAX_COLS;
    first_col = 0;
    current_video = video;
}

void left_screen(){
    width = MAX_COLS/2-1;
    first_col = 0;
    current_video = video + 2*first_col;
}

void right_screen(){
    width = MAX_COLS/2-1;
    first_col = MAX_COLS/2+1;
    current_video = video + 2*first_col;
}