// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clear.h>
#include <divzero.h>
#include <fibonacci.h>
#include <help.h>
#include <inforeg.h>
#include <invopcode.h>
#include <kerberos.h>
#include <kstdbool.h>
#include <kstdio.h>
#include <kstring.h>
#include <primes.h>
#include <printmem.h>
#include <sleeptest.h>
#include <test_inforeg.h>
#include <time.h>
#include <testsync.h>
#include <printsems.h>

#define LINE_LENGTH    512
#define TOKEN_LENGTH   512
#define MAX_PROC_COUNT 2

#define PROMPT_SYMBOL  '>'
#define PIPE_SYMBOL    '|'

#define BACKSPACE_KEY  8
#define PAUSE_KEY      'p'
#define SIGINT_KEY     'c'
#define FOCUS_KEY      9

typedef struct process {
    char name[TOKEN_LENGTH];
    char arg[TOKEN_LENGTH];
    int pid;
    chldstatus_t status;
} process_t;

static process_t children[MAX_PROC_COUNT];
static int fidx = -1;

static enum exit_status {
    FAILURE,
    SUCCESS
};

typedef enum layout_mode {
    FULLSCREEN = 0,
    SPLITSCREEN
} layout_mode_t;

static layout_mode_t current_layout_mode = FULLSCREEN;

static char ctrl_pressed = 0;

// temporary workaround
void invalid_command(char *cmd_name) {
    printf("[ Command %s not found ]\n", cmd_name);
}

static int run_command(char *name, char *arg) {
    if (strcmp(name, "help") == 0)
        return _run(help, arg);

    else if (strcmp(name, "fibonacci") == 0)
        return _run(fibonacci, NULL);

    else if (strcmp(name, "primes") == 0)
        return _run(primes, NULL);

    else if (strcmp(name, "time") == 0)
        return _run(time, NULL);

    else if (strcmp(name, "divzero") == 0)
        return _run(divzero, NULL);

    else if (strcmp(name, "kerberos") == 0)
        return _run(kerberos, NULL);

    else if (strcmp(name, "invopcode") == 0)
        return _run(invopcode, NULL);

    else if (strcmp(name, "inforeg") == 0)
        return _run(inforeg, NULL);

    else if (strcmp(name, "test-inforeg") == 0)
        return _run(testinforeg, NULL);

    else if (strcmp(name, "printmem") == 0)
        return _run(printmem, arg);
    else if (strcmp(name, "testsync") == 0){
        return _run(test_sync, "1");
    }
    else if (strcmp(name, "testnosync") == 0){
        return _run(test_sync, "0");
    }
    else if (strcmp(name, "sleeptest") == 0){
        return _run(sleeptest, NULL);
    }
    else if (strcmp(name, "printsems") ==0){
        return _run(printsems, NULL);
    }

    else if (strcmp(name, "clear") == 0) {
        // temporary workaround.
        // clear command should not be used with pipe operator
        _clear_screen();
        _switch_screen_mode(FULLSCREEN);
        current_layout_mode = FULLSCREEN;
        return 256;
    }

    return _run(invalid_command, name);
}

/**
 * @brief Get token using a state machine.
 * Inspired in John Bode's answer
 *
 * @see https://stackoverflow.com/questions/4547570/tokenizing-a-string-in-c
 */
static int gettoken(char **src, char *token, char delimiter) {
    enum {
        START,
        STEP,
        ERROR,
        DONE
    } state = START;

    size_t i = 0;

    if (**src == '\0')
        return -1;

    while (**src != '\0') {
        switch (state) {
            case START:
                if (**src != delimiter) {
                    token[i++] = *(*src)++;
                    state = STEP;
                } else {
                    (*src)++;
                }
                break;
            case STEP:
                if (**src != delimiter) {
                    token[i++] = *(*src)++;
                } else {
                    token[i++] = '\0';
                    (*src)++;
                    state = DONE;
                }
                break;

            case DONE:
                return SUCCESS;

            case ERROR:
                return FAILURE;
        }
    }

    token[i++] = '\0';
    return SUCCESS;
}

static int parse_command(char **src, char *main, char *arg) {
    enum {
        START,
        COMMAND,
        ARGUMENT,
        ERROR,
        DONE
    } state = START;

    char token[TOKEN_LENGTH];

    // flush buffers
    main[0] = '\0';
    arg[0] = '\0';

    while (1) {
        int res = gettoken(&src, token, ' ');
        if (res == SUCCESS) {
            switch (state) {
                case START:
                    state = COMMAND;
                    strcpy(main, token);
                    break;

                case COMMAND:
                    state = ARGUMENT;
                    strcpy(arg, token);
                    break;

                case ARGUMENT:
                    state = DONE;
                    break;

                case DONE:
                    return SUCCESS;

                case ERROR:
                    return FAILURE;
            }
        } else if (res == -1)
            break;
    }
    return SUCCESS;
}

static void switch_layout(layout_mode_t mode) {
    if (current_layout_mode != mode) {
        current_layout_mode = mode;
        _switch_screen_mode(mode);
    }
}

static void lock_screen() {
    if (current_layout_mode == SPLITSCREEN)
        getchar();
}

static void read_input(char *buffer) {
    char c;
    unsigned int offset = 0;
    printf("%c", PROMPT_SYMBOL);
    while ((c = getchar()) != '\n') {
        if (c == BACKSPACE_KEY) {
            if (offset) {
                _delete_char();
                offset--;
            }
        } else {
            putchar(c);
            buffer[offset++] = c;
        }
    }
    buffer[offset] = '\0';
    printf("\n");
}

void focus_next() {
    do
        fidx = (fidx + 1) % MAX_PROC_COUNT;
    while (children[fidx].status == TERMINATED);
    _focus(children[fidx].pid);
}

bool chld_running() {
    for (size_t i = 0; i < MAX_PROC_COUNT; i++) {
        if (children[i].status == RUNNING)
            return true;
    }
    return false;
}

void sigint_msg() {
    printf("\n[ process terminated ]\n");
}

int shell() {
    char cmd_buff[LINE_LENGTH], token_buff[TOKEN_LENGTH];
    _cntrl_listener(&ctrl_pressed);

    kerberos(); // show welcome screen

    while (1) {
        if (!chld_running()) {
            lock_screen();
            switch_layout(FULLSCREEN);

            read_input(cmd_buff);
            char *input = cmd_buff;

            if (cmd_buff[0] == '\0')
                continue;

            unsigned int command_count = 0;
            while (gettoken(&input, token_buff, PIPE_SYMBOL) != -1) {
                if (command_count < MAX_PROC_COUNT) {
                    parse_command(&token_buff, children[command_count].name,
                                  children[command_count].arg);
                    token_buff[0] = '\0';
                    command_count++;
                }
            }

            if (command_count > 0) {
                switch_layout(command_count - 1);

                // fullscreen: the focused process is the one that
                // is running splitscreen: visual feedback is
                // enabled after first key press
                if (current_layout_mode == FULLSCREEN)
                    fidx = 0;
                else
                    fidx = -1;

                for (size_t i = 0; i < command_count; i++) {
                    children[i].status = RUNNING;
                    children[i].pid =
                        run_command(children[i].name, children[i].arg);
                }
            }
        } else {
            char c;
            if (ctrl_pressed && (c = getchar())) {
                if (fidx != -1 && c == SIGINT_KEY) {
                    _kill(children[fidx].pid);

                    // when the process is killed, the status
                    // of it should be force updated
                    children[fidx].status = TERMINATED;

                    if (chld_running())
                        focus_next();
                } else if (fidx != -1 && c == PAUSE_KEY)
                    _pause(children[fidx].pid);

                else if (c == FOCUS_KEY)
                    focus_next();
            } else {
                for (size_t i = 0; i < MAX_PROC_COUNT; i++) {
                    _wait(i, &children[i].status);
                }
            }
        }
    }

    return 0;
}
