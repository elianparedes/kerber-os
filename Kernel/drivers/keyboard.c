// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <lib/defs.h>
#include <scheduler.h>

#define KBD_ENCODER_PORT 0x60
#define KBD_CTRL_PORT    0x64
/// 0x64 mask status register
enum KBD_CTRL_MASK {

    KBD_CTRL_MASK_OUT_BUF = 1,    // 00000001
    KBD_CTRL_MASK_IN_BUF = 2,     // 00000010
    KBD_CTRL_MASK_SYSTEM = 4,     // 00000100
    KBD_CTRL_MASK_CMD_DATA = 8,   // 00001000
    KBD_CTRL_MASK_LOCKED = 0x10,  // 00010000
    KBD_CTRL_MASK_AUX_BUF = 0x20, // 00100000
    KBD_CTRL_MASK_TIMEOUT = 0x40, // 01000000
    KBD_CTRL_MASK_PARITY = 0x80   // 10000000
};
enum KBD_CTRL_CMD {
    KBD_CTRL_CMD_DISABLE = 0xAD,
    KBD_CTRL_CMD_ENABLE = 0xAE
};

#define KBD_SIZE           90

#define MAYUS_OFFSET       ('a' - 'A')
#define IS_ASCII_LETTER(l) (l >= 'a' && l <= 'z')

#define LSHIFT_MK          0x2A
#define LSHIFT_BK          0xAA
#define RSHIFT_MK          0x36
#define RSHIFT_BK          0xB6
#define CAPS_MK            0x3A
#define LCNTRL_MK          0x1D
#define LCNTRL_BK          0X9D

// CONTROL + R save registers
#define SAVE_REG_KEY 'R'

static char buffer[BUFFER_SIZE];
static uint16_t index = 0;
static uint8_t caps_locked = 0;
static uint8_t shift_pressed = 0;
static uint8_t cntrl_pressed = 0;
static uint8_t *cntrl_listener = &cntrl_pressed;

static uint8_t ctrl_d_flag = 0;

static char kbd_US_1[KBD_SIZE] = {
    0,
    27 /* Esc */,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '\b' /* Backspace */,
    '\t', /* Tab */
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n' /* Enter */,
    0, /* control key */
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    0,
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
    0,
    '*',
    0,   /* Alt */
    ' ', /* Space bar */
    0,   /* Caps lock */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* F1 ... F10 keys */
    0, /* Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up Arrow */
    0, /* Page Up */
    '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
    '+',
    0, /* End key*/
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert Key */
    0, /* Delete Key */
    0,
    0,
    0,
    0, /* F11 Key */
    0, /* F12 Key */
    0, /* Undefined Keys*/
};

static char kbd_shift_US_1[KBD_SIZE] = {
    0,   0,   /*Numbers shift locked*/ '|',
    '@', '#', '$',
    '%', '^', '&',
    '*', '(', ')',
    '_', '+', 0,
    0, /* Backspace and tab */
    'Q', 'W', 'E',
    'R', 'T', 'Y',
    'U', 'I', 'O',
    'P', '{', '}',
    0,   0, /*Enter and control key*/
    'A', 'S', 'D',
    'F', 'G', 'H',
    'J', 'K', 'L',
    ':', '"', '~',
    0,   0,   'Z',
    'X', 'C', 'V',
    'B', 'N', 'M',
    '<', '>', '?',
    0,   0,   0, /* Alt */
    ' ',         /* Space bar */
    0,           /* Caps lock */
    0,   0,   0,
    0,   0,   0,
    0,   0,   0,
    0,      /* F1 ... F10 keys */
    0,      /* Num lock */
    0,      /* Scroll Lock */
    0,      /* Home key */
    0,      /* Up Arrow */
    0,      /* Page Up */
    0,   0, /* Left Arrow */
    0,   0, /* Right Arrow */
    0,   0, /* End key*/
    0,      /* Down Arrow */
    0,      /* Page Down */
    0,      /* Insert Key */
    0,      /* Delete Key */
    0,   0,   0,
    0, /* F11 Key */
    0, /* F12 Key */
    0, /* Undefined Keys*/
};

void kbd_send_enc_cmd(uint8_t cmd) {
    while (1) {
        if ((inb(KBD_CTRL_PORT) & KBD_CTRL_MASK_IN_BUF) == 0)
            break;
    }
    outb(KBD_ENCODER_PORT, cmd);
}

void kbd_send_ctrl_cmd(uint8_t cmd) {
    while (1) {
        if ((inb(KBD_CTRL_PORT) & KBD_CTRL_MASK_IN_BUF) == 0)
            break;
    }
    outb(KBD_CTRL_PORT, cmd);
}

void kbd_disable() {
    kbd_send_ctrl_cmd(KBD_CTRL_CMD_DISABLE);
}

void kbd_enable() {
    kbd_send_ctrl_cmd(KBD_CTRL_CMD_ENABLE);
}

static void ctrl_c_handler() {
    int pid = get_current_process()->pid;
    if (pid != 0)
        kill_process(pid);
}

static void ctrl_d_handler() {
    ctrl_d_flag = 1;
}

void kbd_handler() {

    uint8_t scan_code = inb(KBD_ENCODER_PORT);
    if ((scan_code == LSHIFT_MK) || (scan_code == RSHIFT_MK)) {
        shift_pressed = 1;
        return;
    }
    if ((scan_code == LSHIFT_BK) || (scan_code == RSHIFT_BK)) {
        shift_pressed = 0;
        return;
    }
    if (scan_code == LCNTRL_MK) {
        cntrl_pressed = 1;
        (*cntrl_listener) = cntrl_pressed;
        return;
    }
    if (scan_code == LCNTRL_BK) {
        cntrl_pressed = 0;
        (*cntrl_listener) = cntrl_pressed;
        return;
    }
    if (scan_code == CAPS_MK) {
        caps_locked = !caps_locked;
        return;
    }
    if (index > BUFFER_SIZE || scan_code > KBD_SIZE) {
        return;
    }
    uint8_t character;
    if (shift_pressed) {
        character = kbd_shift_US_1[scan_code];
    } else {
        character = kbd_US_1[scan_code];
    }
    if (character == 0) {
        return; // no character to map
    }
    if (caps_locked && IS_ASCII_LETTER(character)) {
        buffer[index] = character - MAYUS_OFFSET;
    } else if (cntrl_pressed) {
        switch (character) {
            case 'c':
                ctrl_c_handler();
                break;

            case 'd':
                ctrl_d_handler();

            default:
                break;
        }

    } else {
        buffer[index] = character;
    }
    index++;
    buffer[index] = 0;
}

int kbd_get_buffer(char *buffer_ret) {
    if (ctrl_d_flag) {
        ctrl_d_flag = 0;
        return EOF;
    }
    memcpy(buffer_ret, buffer, index);
    return 0;
}

void kbd_clear_buffer() {
    buffer[0] = 0;
    index = 0;
}

uint16_t kbd_get_current_index() {
    return index;
}

char kbd_get_last_key() {
    if (index > 0) {
        return buffer[index - 1];
    }
    return 0;
}

char kbd_get_rm_last_key() {
    if (index > 0) {
        char toReturn = buffer[index - 1];
        index--;
        buffer[index] = 0;
        return toReturn;
    }
    return 0;
}

void kbd_sets_cntrl_listener(uint8_t *listener) {
    cntrl_listener = listener;
}

uint8_t kbd_is_cntrl_pressed() {
    return cntrl_pressed;
}

uint8_t kbd_is_save_reg_shortcut() {
    if (cntrl_pressed &&
        ((kbd_get_last_key() == SAVE_REG_KEY) ||
         ((kbd_get_last_key() - MAYUS_OFFSET) == SAVE_REG_KEY))) {
        kbd_get_rm_last_key();
        return 1;
    }
    return 0;
}