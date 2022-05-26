enum colors {BLACK = 0,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,LIGHT_GRAY,
DARK_GRAY,LIGHT_BLUE,LIGHT_GREEN,LIGHT_CYAN,LIGHT_RED,LIGHT_MAGENTA,YELLOW,WHITE};

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void disable_cursor();

void move_cursor(int row , int col);

void get_cursor_position(int pos[2]);

void print_string(const char * string);

void print_char(const char c);

/**
 *TODO: 
 **/
void print_new_line();

/**
 *TODO: 
 **/
void clear_screen();

/**
 *TODO: 
 **/
void clear_line(int row);

/**
 *TODO: 
 **/
void select();

void scroll(int lines);

void set_background_color(int first_row , int first_col , int last_row , int last_col, enum colors color);

void set_foreground_color(int first_row , int first_col , int last_row , int last_col, enum colors color);