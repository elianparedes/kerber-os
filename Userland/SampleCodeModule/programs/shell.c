// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clear.h>
#include <divzero.h>
#include <fibonacci.h>
#include <help.h>
#include <inforeg.h>
#include <invopcode.h>
#include <kerberos.h>
#include <kmman.h>
#include <kstdbool.h>
#include <kstdio.h>
#include <kstring.h>
#include <primes.h>
#include <printmem.h>
#include <printsems.h>
#include <schd.h>
#include <sleeptest.h>
#include <test_inforeg.h>
#include <testmm.h>
#include <testsync.h>
#include <time.h>
#include <infopipe.h>
#include <test_pipe.h>
#include <ipc_programs.h>
#include <printsems.h>
#include <printmemstate.h>

#define LINE_LENGTH    512
#define TOKEN_LENGTH   512
#define MAX_PROC_COUNT 2
#define MAX_ARGC       5

#define PROMPT_SYMBOL  '>'
#define PIPE_SYMBOL    '|'

#define BACKSPACE_KEY  8
#define PAUSE_KEY      'p'
#define SIGINT_KEY     'c'
#define FOCUS_KEY      9

typedef enum layout_mode {
    FULLSCREEN = 0,
    SPLITSCREEN
} layout_mode_t;

typedef struct cmd {
    char *name;
    char *argv[MAX_ARGC];
    int argc;
} cmd_t;

typedef struct line {
    char operator; // '|' or '&'
    cmd_t *left_cmd;
    cmd_t *right_cmd;
} line_t;

static char whitespace[] = " \t\n";
static char operators[] = "|&";

static layout_mode_t current_layout_mode = FULLSCREEN;

static char ctrl_pressed = 0;

// temporary workaround
static void invalid_command(char *cmd_name) {
    printf("[ Command %s not found ]\n", cmd_name);
}

static int run_command(char *name, int argc, char *argv[]) {
    if (strcmp(name, "help") == 0) {
        return _run(help, argc, argv);
    }

    else if (strcmp(name, "fibonacci") == 0)
        return _run(fibonacci, argc, argv);

    else if (strcmp(name, "primes") == 0)
        return _run(primes, argc, argv);

    else if (strcmp(name, "time") == 0)
        return _run(time, argc, argv);

    else if (strcmp(name, "divzero") == 0)
        return _run(divzero, argc, argv);

    else if (strcmp(name, "kerberos") == 0)
        return _run(kerberos, argc, argv);

    else if (strcmp(name, "invopcode") == 0)
        return _run(invopcode, argc, argv);

    else if (strcmp(name, "inforeg") == 0)
        return _run(inforeg, argc, argv);

    else if (strcmp(name, "testinforeg") == 0)
        return _run(testinforeg, argc, argv);

    else if (strcmp(name, "printmem") == 0)
        return _run(printmem, argc, argv);
    else if (strcmp(name, "mem") == 0){
        return _run(printmemstate, 0, NULL);
    }
    else if (strcmp(name, "testsync") == 0){
        return _run(test_sync, argc, argv);
    }
    else if (strcmp(name, "testnosync") == 0){
        return _run(test_sync, argc, argv);
    }

    else if (strcmp(name, "sleeptest") == 0)
        return _run(sleeptest, argc, argv);

    else if (strcmp(name, "sem") == 0)
        return _run(printsems, argc, argv);

    else if (strcmp(name, "sched") == 0)
        return _run(schd, argc, argv);
    else if (strcmp(name, "pipe") == 0)
        return _run(info_all_pipes, 0, NULL);

    else if (strcmp(name, "cat") == 0)
        return _run(cat, 0, NULL);

    else if (strcmp(name, "filter") == 0)
        return _run(filter,0,NULL);

    else if (strcmp(name, "wc") == 0)
        return _run(wc,0,NULL);

    else if (strcmp(name, "testpipes") == 0)
        return _run(test_pipes, 0, NULL);

    else if (strcmp(name, "testmm") == 0)
        return _run(test_mm, argc, argv);

    else if (strcmp(name, "clear") == 0) {
        // temporary workaround.
        // clear command should not be used with pipe operator
        _clear_screen();
        _switch_screen_mode(FULLSCREEN);
        current_layout_mode = FULLSCREEN;
        return 256;
    }

    printf("[ Command %s not found ]\n", name);
    return -1;
}

/**
 * @brief Get token using a state machine.
 * Inspired in John Bode's answer
 *
 * @see https://stackoverflow.com/questions/4547570/tokenizing-a-string-in-c
 */
static int gettoken(char **src, char *token, char *delimiters) {
    enum {
        START,
        STEP,
        DELIM,
        ERROR
    } state = START;

    size_t i = 0;

    if (**src == '\0')
        return -1;

    while (**src != '\0') {
        switch (state) {
            case START:
                // ignore whitespaces
                if (strchr(whitespace, **src))
                    (*src)++;

                // if delimiter is found at first, go to error
                else if (strchr(delimiters, **src))
                    state = ERROR;

                // if letter is found, go to next step
                else
                    state = STEP;

                break;
            case STEP:
                // fill the buffer until delimiter
                if (!strchr(delimiters, **src))
                    token[i++] = *(*src)++;

                else
                    state = DELIM;

                break;
            case DELIM:
                token[i] = '\0';
                return *(*src)++;
            case ERROR:
                return -1;
            default:
                break;
        }
    }

    token[i] = '\0';
    return *(*src);
}

static cmd_t *getcmd(char *input) {
    char *cmdidx = input;
    char token[TOKEN_LENGTH] = {0};

    cmd_t *cmd = _malloc(sizeof(cmd_t));
    cmd->argc = 0;

    // get the name of the command;
    gettoken(&cmdidx, token, whitespace);
    cmd->name = _malloc(sizeof(strlen(token)) + 1);
    strcpy(cmd->name, token);
    token[0] = '\0';

    while (gettoken(&cmdidx, token, whitespace) != -1 && cmd->argc < MAX_ARGC) {
        cmd->argv[cmd->argc] = _malloc(sizeof(strlen(token)) + 1);
        strcpy(cmd->argv[cmd->argc], token);
        cmd->argc++;
        token[0] = '\0';
    }

    return cmd;
}

static line_t *parseline(char *line) {

    char *l = line;
    char chunk[TOKEN_LENGTH] = {0};

    line_t *pline = _malloc(sizeof(line_t));
    pline->left_cmd = NULL;
    pline->right_cmd = NULL;
    pline->operator= 0;

    int delim = 0;

    while ((delim = gettoken(&l, chunk, operators)) != -1) {
        switch (delim) {
            case '|':
                if (pline->operator)
                    break;

                pline->operator= '|';
                pline->left_cmd = getcmd(chunk);
                break;
            case '&':
                if (pline->operator)
                    break;

                pline->operator= '&';
                pline->left_cmd = getcmd(chunk);
                break;
            case '\0':
                if (pline->operator== '|')
                    pline->right_cmd = getcmd(chunk);

                else if (pline->operator!= '&')
                    pline->left_cmd = getcmd(chunk);

                break;
            default:
                break;
        }
    }

    return pline;
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

void printcmd(cmd_t *cmd) {
    printf("cmd: %s\n", cmd->name);
    printf("argc: %d\n", cmd->argc);
    for (int i = 0; i < cmd->argc; ++i) {
        printf("arg[%d]: %s\n", i, cmd->argv[i]);
    }
    printf("\n");
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

void sigint_msg() {
    printf("\n[ process terminated ]\n");
}

void freecmd(cmd_t *cmd) {
    if (cmd == NULL)
        return;

    for (size_t i = 0; i < cmd->argc; i++)
        _free(cmd->argv[i]);
}

void freepline(line_t *parsedline) {
    freecmd(parsedline->left_cmd);
    freecmd(parsedline->right_cmd);
    _free(parsedline);
}

int shell() {
    char cmd_buff[LINE_LENGTH], token_buff[TOKEN_LENGTH];
    _cntrl_listener(&ctrl_pressed);

    kerberos(); // show welcome screen

    while (1) {
        lock_screen();
        switch_layout(FULLSCREEN);

        read_input(cmd_buff);
        char *input = cmd_buff;

        if (cmd_buff[0] == '\0')
            continue;

        line_t *parsedline = parseline(input);
        switch (parsedline->operator) {
            case '|':
                printf("operator: %c\n", parsedline->operator);
                printcmd(parsedline->left_cmd);
                printcmd(parsedline->right_cmd);
                break;
            case '&':
                run_command(parsedline->left_cmd->name,
                            parsedline->left_cmd->argc,
                            parsedline->left_cmd->argv);
                break;

            default:
                run_command(parsedline->left_cmd->name,
                            parsedline->left_cmd->argc,
                            parsedline->left_cmd->argv);
                _wait2();
                break;
        }

        freepline(parsedline);
    }

    return 0;
}
