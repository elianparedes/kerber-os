#include <video.h>

#define VIDEO_ADDRESS                 0xB8000

#define CRTC_ADRESS_PORT              0x3D4
#define CRTC_DATA_PORT                0x3D5

#define CURSOR_START_REGISTER         0x0A
#define CURSOR_END_REGISTER           0x0B

#define CURSOR_LOCATION_HIGH_REGISTER 0x0E
#define CURSOR_LOCATION_LOW_REGISTER  0x0F

#define CURSOR_MAX_HEIGHT             15
#define VIDEO_OFFSET()                (current_video - video)
#define CURRENT_COL()                 (VIDEO_OFFSET() % (2 * MAX_COLS))
#define CURRENT_LINE()                (VIDEO_OFFSET() / (2 * MAX_COLS))
#define LINE_ADDRESS(LINE)            ((LINE)*2 * MAX_COLS + video)

static uint8_t *video = (uint8_t *)VIDEO_ADDRESS;
static uint8_t *current_video = (uint8_t *)VIDEO_ADDRESS;

static area_t work_area = {
    .width = MAX_COLS, .height = MAX_ROWS, .first_col = 0, .first_row = 0};

static int cursor_row = 0;
static int cursor_col = 0;
static int cursor_enable = 0;
static int scroll_enable = 0;
static void update_current_video() {
    current_video = video + cursor_row * 2 * MAX_COLS + cursor_col * 2;
}

static void update_cursor_position() {

    if (!cursor_enable)
        return;

    cursor_col = CURRENT_COL() / 2;
    cursor_row = CURRENT_LINE();
    uint16_t pos = cursor_row * MAX_COLS + cursor_col;

    outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_LOW_REGISTER);
    outb(CRTC_DATA_PORT, (uint8_t)pos);
    outb(CRTC_ADRESS_PORT, CURSOR_LOCATION_HIGH_REGISTER);
    outb(CRTC_DATA_PORT, (uint8_t)(pos >> 8));
}


void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(CRTC_ADRESS_PORT, CURSOR_START_REGISTER);
    outb(CRTC_DATA_PORT, (inb(CRTC_DATA_PORT) & 0xC0) | cursor_start);

    outb(CRTC_ADRESS_PORT, CURSOR_END_REGISTER);
    outb(CRTC_DATA_PORT, (inb(CRTC_DATA_PORT) & 0xA0) | cursor_end);
    cursor_enable = 1;
    update_cursor_position();
}

void disable_cursor() {
    outb(CRTC_ADRESS_PORT, CURSOR_START_REGISTER);
    outb(CRTC_DATA_PORT, 0x20);
    cursor_enable = 0;
}

void move_cursor(position_t pos) {
    if (!cursor_enable)
        return;
    cursor_col = pos.col;
    cursor_row = pos.row;
    update_current_video();
    update_cursor_position();
}

position_t get_cursor_position() {
    position_t pos;
    pos.col = cursor_col;
    pos.row = cursor_row;
    return pos;
}

void print_string(const char *string) {
    for (int i = 0; string[i] != 0; i++) {
        print_char(string[i]);
    }
}

void print_char(const char c) {

    int current_col = CURRENT_COL();
    int current_line = CURRENT_LINE();
    if (current_col == (2 * (work_area.first_col + work_area.width - 1)) &&
        (work_area.height == 1)) {
        clear_line(current_line);
        position_t pos = {current_line, work_area.first_col};
        move_cursor(pos);
        *current_video = c;
    } else {
        *current_video = c;
        int is_last_col =
            (current_col == 2 * (work_area.width + work_area.first_col - 1));
        int is_last_row =
            (current_line == (work_area.first_row + work_area.height - 1));

        if (is_last_col && !is_last_row) {
            print_new_line();
            return;
        }

        if (is_last_col && is_last_row) {
            scroll();
            return;
        }
    }

    current_video += 2;
    update_cursor_position();
}

void print_new_line() {

    int current_line = CURRENT_LINE();

    if (current_line == (work_area.first_row + work_area.height - 1))
        scroll();
    else
        current_line++;
    current_video = LINE_ADDRESS(current_line) + 2 * work_area.first_col;
    update_cursor_position();
}

void delete_char() {

    if (!cursor_enable)
        return;

    int current_line = CURRENT_LINE();
    int current_col = CURRENT_COL();

    current_video -= 2;
    current_video[0] = 0;

    if (current_col == work_area.first_col) {
        if (current_line != 0)
            current_video = LINE_ADDRESS(current_line - 1) +
                            2 * (work_area.width + work_area.first_col -
                                 ((work_area.width == MAX_COLS) ? 1 : 0));
        else
            return;
    }
    update_cursor_position();
}

void clear_screen() {
    for (int i = work_area.first_row;
         i < work_area.height + work_area.first_row; i++) {
        clear_line(i);
    }
    current_video =
        video + work_area.first_col * 2 + work_area.first_row * 2 * MAX_COLS;
    update_cursor_position();
}

void clear_line(int line) {
    uint8_t *current_line_video = LINE_ADDRESS(line) + 2 * work_area.first_col;
    for (int i = 0; i < work_area.width * 2; i++) {
        if (!(i % 2))
            current_line_video[i] = 0;
    }
}

void scroll() {
    if (!scroll_enable)
        return;
    for (int i = work_area.first_row + 1;
         i < (work_area.height + work_area.first_row); i++) {
        uint8_t *current_line_video = LINE_ADDRESS(i) + 2 * work_area.first_col;
        for (int j = 0; j < work_area.width * 2; j++) {
            current_line_video[j - 2 * MAX_COLS] = current_line_video[j];
        }
    }
    clear_line(work_area.first_row + work_area.height - 1);
    current_video = LINE_ADDRESS(work_area.first_row + work_area.height - 1) +
                    2 * work_area.first_col;
    update_cursor_position();
}

void set_working_area(area_t area) {
    if (area.first_row + area.height > MAX_ROWS) {
        if (area.first_row >= MAX_ROWS)
            return;
        area.height = MAX_ROWS - area.first_row;
    }
    work_area.width = area.width;
    work_area.height = area.height;
    work_area.first_col = area.first_col;
    work_area.first_row = area.first_row;
    current_video =
        video + 2 * work_area.first_row * MAX_COLS + 2 * work_area.first_col;
}

void full_screen() {
    work_area.width = MAX_COLS;
    work_area.first_col = 0;
    current_video = video;
}

void left_screen() {
    work_area.width = MAX_COLS / 2 - 1;
    work_area.first_col = 0;
    current_video = video + 2 * work_area.first_col;
}

void right_screen() {
    work_area.width = MAX_COLS / 2 - 1;
    work_area.first_col = MAX_COLS / 2 + 1;
    current_video = video + 2 * work_area.first_col;
}

void enable_scroll() {
    scroll_enable = 1;
}

void disable_scroll() {
    scroll_enable = 0;
}

static void set_color(int first_row, int first_col, int last_row, int last_col,
                      enum colors color, int is_background) {
    uint8_t *aux = (video + 1);
    aux += first_row * (MAX_COLS * 2) + (first_col * 2);
    int row_delta = last_row - first_row;
    int col_delta = last_col - first_col;
    int row_counter = 0;
    int col_counter = 0;
    while (row_counter <= row_delta) {
        while (col_counter <= col_delta) {
            *aux = is_background ? ((color << 4) | ((*aux) & 0x0F))
                                 : ((color & 0x0F) | ((*aux) & 0xF0));
            aux += 2;
            col_counter++;
        }
        aux -= 2;
        aux += (2 * MAX_COLS - 2 * (last_col - first_col));
        col_counter = 0;
        row_counter++;
    }
}

void set_background_color(int first_row, int first_col, int last_row,
                          int last_col, enum colors color) {
    set_color(first_row, first_col, last_row, last_col, color, 1);
}

void set_foreground_color(int first_row, int first_col, int last_row,
                          int last_col, enum colors color) {
    set_color(first_row, first_col, last_row, last_col, color, 0);
}

area_t get_current_working_area() {
    return work_area;
}