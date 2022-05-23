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

/**
 * TODO: Incluir este link que es de donde "copiamos" el codigo
 * https://wiki.osdev.org/Text_Mode_Cursor#Moving_the_Cursor_2
 **/

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end){
	outb(CRTC_ADRESS_PORT, CURSOR_START_REGISTER);
	outb(CRTC_DATA_PORT, (inb(CRTC_DATA_PORT) & 0xC0) | cursor_start);

	outb(CRTC_ADRESS_PORT, CURSOR_END_REGISTER);
	outb(CRTC_DATA_PORT, (inb(CRTC_DATA_PORT) & 0xA0) | cursor_end );
}

void disable_cursor(){
	outb(CRTC_ADRESS_PORT, CURSOR_START_REGISTER);
	outb(CRTC_DATA_PORT, 0x20);
}


void move_cursor(int row, int col){
	uint16_t pos = row* MAX_COLS + col;
 
	outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_LOW_REGISTER);
	outb(CRTC_DATA_PORT, (uint8_t) pos );
	outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_HIGH_REGISTER);
	outb(CRTC_DATA_PORT, (uint8_t)(pos >> 8));
}

void get_cursor_position(int pos[2]){
    outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_LOW_REGISTER);
    uint16_t low_pos = inb(CRTC_DATA_PORT);
    outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_HIGH_REGISTER);
    uint16_t high_pos = inb(CRTC_DATA_PORT);
    uint16_t position = (high_pos << 8) + low_pos; 
    pos[0] = position/(MAX_COLS);
    pos[1] = position%(MAX_COLS);
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
    *current_video = c;
    current_video+=2;
}

/**
 * TODO: Al pasar 25 de scroll con pantalla dividida en 2 colores (horizontalmente) en lugar de mantener la logica de arrastrar
 * el ultimo color, simplemente limpia todos los caracteres y mantiene los colores originales. Para un clear tiene sentido, pero no
 * para un scroll con este comprotamiento de arrastre
 **/

void scroll(int lines){
    
    int index = 0;
    int delta = MAX_COLS*2;
    if(lines == 25){
        index = 0;
        delta = 0;
    } else {
        for(int i = 0 ; i < MAX_ROWS-(lines-1) ; i++){
            index = i*delta;
            if(lines != 25){
                for(int j = 0 ; j < delta ; j++){
                    video[j+index] = video[j+(delta*lines)+index];
                }
            }
        }
    }

    while(lines > 0){
        
        for(int j = 0 ; j < MAX_COLS*2 ; j++ ){
            uint8_t color = video[j+index-delta];
                video[j+index] = (j%2 ? color : ' ');
        }
        index += 2*MAX_COLS;
        lines--;
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

void set_background_color(int first_row , int first_col , int last_row , int last_col, enum colors color){
    set_color(first_row,first_col,last_row,last_col,color,1);
}

void set_foreground_color(int first_row , int first_col , int last_row , int last_col, enum colors color){
    set_color(first_row,first_col,last_row,last_col,color,0);
}