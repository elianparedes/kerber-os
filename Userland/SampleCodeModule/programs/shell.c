#include <clear.h>
#include <divzero.h>
#include <fibonacci.h>
#include <help.h>
#include <inforeg.h>
#include <invalidopcode.h>
#include <kerberos.h>
#include <kstdio.h>
#include <kstring.h>
#include <primes.h>
#include <printmem.h>
#include <time.h>

#define LINE_LENGTH 512
#define TOKEN_LENGTH 512
#define MAX_PROC_COUNT 2

#define PROMPT_SYMBOL "> "
#define PIPE_SYMBOL '|'

#define BACKSPACE_KEY 8
#define PAUSE_KEY 'p'
#define SIGINT_KEY 'c'
#define FOCUS_KEY 9

typedef struct process {
    char name[TOKEN_LENGTH];
    char main[TOKEN_LENGTH];
    char arg[TOKEN_LENGTH];
    int pid;
} process_t;

static enum exit_status { FAILURE, SUCCESS };

typedef enum layout_mode { FULLSCREEN = 0, SPLITSCREEN } layout_mode_t;

static layout_mode_t current_layout_mode = FULLSCREEN;

static char ctrl_pressed = 0;

/**
 * @brief Get token using a state machine.
 * Inspired in John Bode's answer
 *
 * @see https://stackoverflow.com/questions/4547570/tokenizing-a-string-in-c
 */
static int gettoken(char **src, char *token, char delimiter) {
    enum { START, STEP, ERROR, DONE } state = START;

    size_t i = 0;

    if (**src == '\0') return -1;

    while (**src != '\0') {
        switch (state) {
            case START:
                if (**src != delimiter) {
                    token[i++] = *(*src)++;
                    state = STEP;
                } else {
                    *(*src)++;
                }
                break;
            case STEP:
                if (**src != delimiter) {
                    token[i++] = *(*src)++;
                } else {
                    token[i++] = '\0';
                    *(*src)++;
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
    enum { START, COMMAND, ARGUMENT, ERROR, DONE } state = START;
    char token[TOKEN_LENGTH];

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

static void read_input(char *buffer) {
    char c;
    unsigned int offset = 0;
    printf(PROMPT_SYMBOL);
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

static int run_command(char *main) {
    if (strcmp(main, "help") == 0)
        return _run(&help);

    else if (strcmp(main, "fibonacci") == 0)
        return _run(&fibonacci);

    else if (strcmp(main, "primes") == 0)
        return _run(&primes);

    else if (strcmp(main, "time") == 0)
        return _run(&time);

    else if (strcmp(main, "divzero") == 0)
        return _run(&divzero);

    else if (strcmp(main, "kerberos") == 0)
        return _run(&kerberos);

    else if (strcmp(main, "invalidopcode") == 0)
        return _run(&invalidopcode);

    else if (strcmp(main, "inforeg") == 0)
        return _run(&inforeg);

    else if (strcmp(main, "clear") == 0) {
        // temporary workaround. clear command should not be used with pipe
        // operator
        _clear_screen();
        _switch_screen_mode(FULLSCREEN);
        return 1;
    }
    return -1;
}

int shell() {
    char cmd_buff[LINE_LENGTH], token_buff[TOKEN_LENGTH];
    unsigned int pcount = 0;
    process_t children[MAX_PROC_COUNT];
    int fidx = 0;

    _cntrl_listener(&ctrl_pressed);

    kerberos();  // show welcome screen

    while (1) {
        if (pcount == 0) {
            unsigned int command_count = 0;
            read_input(cmd_buff);
            char *input = cmd_buff;

            while (gettoken(&input, token_buff, PIPE_SYMBOL) != -1) {
                if (command_count <= MAX_PROC_COUNT) {
                    parse_command(&token_buff, children[command_count].main,
                                  children[command_count].arg);
                    command_count++;
                }
            }

            switch_layout(command_count - 1);

            for (size_t i = 0; i < command_count; i++) {
                process_t p = children[i];
                int pid = run_command(p.main);
                if (pid >= 0) {
                    children[i].pid = pid;
                    strcpy(p.name, p.main);
                    pcount++;
                } else
                    printf("[command %s not found]\n\n", p.main);
            }

        } else {
            char c;
            if (ctrl_pressed && (c = getchar())) {
                if (c == SIGINT_KEY) {
                    switch_layout(FULLSCREEN);
                    while (pcount) {
                        int pid = children[--pcount].pid;
                        _kill(pid);
                        printf("\n[process %s (%d) terminated]\n",
                               children[pcount].main, pid);
                    }
                    putchar('\n');

                } else if (c == PAUSE_KEY) {
                    for (int i = 0; i < pcount; i++) {
                        int pid = children[i].pid;
                        _pause(pid);
                    }
                } else if (c == FOCUS_KEY) {
                    int fpid = children[fidx++ % pcount].pid;
                    _focus(fpid);
                }
            } else {
                if (!_running(0)) {
                    pcount = 0;
                }
            }
        }
    }

    return 0;
}
