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

static uint8_t * video = (uint8_t *) VIDEO_ADDRESS;
static uint8_t * current_video = (uint8_t *) VIDEO_ADDRESS;
static const uint32_t width = MAX_ROWS;
static const uint32_t height = MAX_COLS;

static int cursor_row = 0;
static int cursor_col = 0;
static int cursor_enable = 0;

enum writting_mode{INSERT_WRITTING_MODE,OVERTYPE_WRITTING_MODE};
static enum writting_mode current_writting_mode = INSERT_WRITTING_MODE;

typedef enum {MOVE_UP = -1 , MOVE_DOWN = 1} ldirection_t;

/**
 * TODO: Incluir este link que es de donde "copiamos" el codigo
 * https://wiki.osdev.org/Text_Mode_Cursor#Moving_the_Cursor_2
 **

  /**
  * TODO: Parametrizar todas las funciones para un tamaño reducido de pantalla (max_rows y max_cols menores o iguales a los del DEFINE)
  * Esto debe permitir scrollear en una unica zona de pantalla
  */

static void update_current_video(){
    current_video = video + cursor_row*2*MAX_COLS + cursor_col*2;
}

static void update_cursor_position(){

    if(!cursor_enable)
        return;

    cursor_col = ((current_video - video)%(2*MAX_COLS))/2;
    cursor_row = (current_video - video) / (2*MAX_COLS);
    uint16_t pos = cursor_row* MAX_COLS + cursor_col;

    outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_LOW_REGISTER);
	outb(CRTC_DATA_PORT, (uint8_t) pos );
	outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_HIGH_REGISTER);
	outb(CRTC_DATA_PORT, (uint8_t)(pos >> 8));

}

 static void copy_line(int init_line , int offset , ldirection_t direction) {
    uint8_t * aux_video_ptr = video;
    aux_video_ptr += init_line * MAX_COLS*2;
    for(int j = 0 ; j < MAX_COLS*2 ; j++){
        aux_video_ptr[j + offset*direction*MAX_COLS*2] = j%2 ? 
            ((aux_video_ptr[j] & 0x0F) | (aux_video_ptr[j + offset*direction*MAX_COLS*2] & 0xF0)) 
            : aux_video_ptr[j]  ;
    }
}

static void copy_lines(int init_line , int count , int offset , ldirection_t direction){
    
    int current_line;
    switch (direction)
    {
    case MOVE_DOWN:
        current_line = init_line + count -1;
        if(init_line + count + offset  > MAX_ROWS ){
            current_line -= (init_line + count + offset  - MAX_ROWS);
            count -= (init_line + count + offset  - MAX_ROWS);
        }
        break;
    
    case MOVE_UP:
        current_line = init_line;
        if(init_line - offset < 0 ){
            current_line += (offset - init_line);
            count -= (offset - init_line);
        }
        break;
    }


    while(count > 0){
        copy_line(current_line,offset,direction);
        current_line += -1*direction;
        count--;
    }

}

static void move_lines(int init_line , int count , int offset , ldirection_t direction){
    copy_lines(init_line,count,offset,direction);
    int current_line;

    switch (direction)
    {
    case MOVE_UP:
        current_line = init_line + count-1;
        
        break;
    
    case MOVE_DOWN:
        current_line = init_line;
        break;
    }

    
    if(offset < count)
        count = offset;
    
    while(count > 0){
        clear_line(current_line);
        current_line += direction;
        count--;
    }
}

static void set_color(int first_row , int first_col , int last_row , int last_col, enum colors color,int is_background){
    uint8_t * aux = (video+1);
    aux += first_row*(MAX_COLS*2) + (first_col*2);
    int row_delta = last_row - first_row;
    int col_delta = last_col - first_col;
    int row_counter = 0;
    int col_counter = 0;
    while(row_counter <= row_delta){
        while(col_counter <= col_delta){
            *aux = is_background ? ((color<<4) | ((*aux) & 0x0F)) : ((color&0x0F) | ((*aux) & 0xF0));
            aux += 2;
            col_counter++;
        }
        aux -= 2;
        aux += (2*MAX_COLS - 2*(last_col-first_col));
        col_counter = 0;
        row_counter++;
    }
}

static void insert(const char c , int current_line , int current_col ){
    uint8_t * aux_video_ptr = video + current_line*(2*MAX_COLS);
    if(aux_video_ptr[2*(MAX_COLS-1)] != 0 ){
        if(current_line == (MAX_ROWS - 1) ){
            scroll();
            current_line--;
        }
        insert(aux_video_ptr[2*(MAX_COLS-1)] , current_line+1 , 0 );
    }
    for(int i = (MAX_COLS-2)*2 ; i >= current_col ; i--){
        aux_video_ptr[i+2] = aux_video_ptr[i]; 
    }
    aux_video_ptr[current_col] = c;
}

static void overtype(const char c){
    *current_video = c;
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
    if((current_video-video)%(2*MAX_COLS) == 0 && (current_video-video)/(2*MAX_COLS) == MAX_ROWS){
        scroll(1);
        current_video -= (2*MAX_COLS);
    }
    switch (current_writting_mode)
    {
    case INSERT_WRITTING_MODE:
        insert(c,(current_video-video)/(2*MAX_COLS),(current_video-video)%(2*MAX_COLS));
        break;
    
    case OVERTYPE_WRITTING_MODE:
        overtype(c);
        break;
    }
    current_video+=2;
    update_cursor_position();
}

void print_new_line(){

    int col = (current_video - video) % (MAX_COLS*2);
    int line = (current_video - video) / (MAX_COLS*2);

    if(col == 0 && line == 0){
        move_lines(line,MAX_ROWS-line,1,MOVE_DOWN);
        current_video = video;
        return;
    }

    if(line == MAX_ROWS-1)
        move_lines(0,1,1,MOVE_UP);
    else
        move_lines(line+1,MAX_ROWS-line,1,MOVE_DOWN);
    
    uint8_t * aux_video_ptr = video + line*2*MAX_COLS;
    current_video = video + (line+1)*2*MAX_COLS;
    update_cursor_position();

    for(int i = col, j = 0 ; i < MAX_COLS*2 - col ; i+=2 , j+=2){
        current_video[j] = aux_video_ptr[i];
        aux_video_ptr[i] = 0;
    }
}

void delete_char(){
    if(!cursor_enable)
        return;

    uint8_t current_line = (current_video - video) / (2*MAX_COLS);
    uint8_t current_col = (current_video - video) % (2*MAX_COLS);
    uint8_t * aux_video_ptr = video + current_line*MAX_COLS*2;
    
    if(current_col == 2*(MAX_COLS-1)){
        aux_video_ptr[current_col] = 0;
        return;
    }

    for(int i = current_col ; i < (2*MAX_COLS) ; i++){
        aux_video_ptr[i] = aux_video_ptr[i+2]; 
    }
    aux_video_ptr[2*(MAX_COLS-1)] = 0;
}

void clear_screen(){
    for(int i = 0 ; i < MAX_ROWS ; i++){
        clear_line(i);
    }
    current_video = video;
}

void clear_line(int line){
    uint8_t * aux_video_ptr = video + line*MAX_COLS*2;
    for(int i = 0 ; i < MAX_COLS*2 ; i++){
        if(!(i%2))
            aux_video_ptr[i] = 0 ;
    }
}

void scroll(){
    move_lines(1,MAX_ROWS-1,1,MOVE_UP);
    current_video -= (((current_video-video)%(2*MAX_COLS) == 0) ? 0 : 2*MAX_COLS);
    update_cursor_position();
}

void set_background_color(int first_row , int first_col , int last_row , int last_col, enum colors color){
    set_color(first_row,first_col,last_row,last_col,color,1);
}

void set_foreground_color(int first_row , int first_col , int last_row , int last_col, enum colors color){
    set_color(first_row,first_col,last_row,last_col,color,0);
}

void set_insert_mode(){
    current_writting_mode = INSERT_WRITTING_MODE;
}

void set_overtype_mode(){
    current_writting_mode = OVERTYPE_WRITTING_MODE;
}