enum colors {BLACK = 0,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,LIGHT_GRAY,
DARK_GRAY,LIGHT_BLUE,LIGHT_GREEN,LIGHT_CYAN,LIGHT_RED,LIGHT_MAGENTA,YELLOW,WHITE};

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void disable_cursor();

void move_cursor(int row , int col);

void get_cursor_position(int pos[2]);

void print_string(const char * string);

void print_char(const char c);

void print_new_line();

void delete_char();

void clear_screen();

void clear_line(int line);

void scroll();

void set_working_area(int first_col , int width);

void full_screen();

void left_screen();

void right_screen();

