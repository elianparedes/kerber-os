// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <block.h>
#include <clear.h>
#include <divzero.h>
#include <fibonacci.h>
#include <help.h>
#include <infopipe.h>
#include <inforeg.h>
#include <invopcode.h>
#include <ipc_programs.h>
#include <kerberos.h>
#include <kmman.h>
#include <ksemaphore.h>
#include <kstdbool.h>
#include <kstdio.h>
#include <kstring.h>
#include <phylo.h>
#include <primes.h>
#include <printmem.h>
#include <printmemstate.h>
#include <printsems.h>
#include <ps.h>
#include <schd.h>
#include <sleeptest.h>
#include <test_inforeg.h>
#include <test_pipe.h>
#include <testmm.h>
#include <testsync.h>
#include <time.h>

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

typedef int (*function_t)(int, char *[]);

typedef struct cmd_entry {
    char *name;
    function_t function;
} cmd_entry_t;

cmd_entry_t cmd_table[] = {{"help", help},
                           {"fibonacci", fibonacci},
                           {"primes", primes},
                           {"time", time},
                           {"divzero", divzero},
                           {"kerberos", kerberos},
                           {"invopcode", invopcode},
                           {"inforeg", inforeg},
                           {"testinforeg", testinforeg},
                           {"printmem", printmem},
                           {"mem", printmemstate},
                           {"testsync", test_sync},
                           {"sleeptest", sleeptest},
                           {"sem", printsems},
                           {"sched", schd},
                           {"pipe", info_all_pipes},
                           {"cat", cat},
                           {"filter", filter},
                           {"wc", wc},
                           {"block", block},
                           {"testpipes", test_pipes},
                           {"testmm", test_mm},
                           {"phylo", phylo},
                           {"ps", ps},
                           {NULL, NULL}};

sem_ptr sem_pipe_exec;

static char whitespace[] = " \t\n";
static char operators[] = "|&";

static layout_mode_t current_layout_mode = FULLSCREEN;

static char ctrl_pressed = 0;

static function_t get_cmd(char *cmd_name) {
    for (int i = 0; cmd_table[i].name != NULL; i++) {
        if (!strcmp(cmd_name, cmd_table[i].name)) {
            return cmd_table[i].function;
        }
    }

    return NULL;
}

// temporary workaround
static void invalid_command(char *cmd_name) {
    printf("[ Command %s not found ]\n", cmd_name);
}

static int run_command(char *name, int argc, char *argv[]) {

    function_t function = get_cmd(name);

    if (function == NULL) {
        printf("[ Command %s not found ]\n", name);
        return -2;
    }

    _run(function, argc, argv);
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
    cmd->name = _malloc(strlen(token) + 1);
    strcpy(cmd->name, token);

    cmd->argv[cmd->argc] = _malloc(strlen(token) + 1);
    strcpy(cmd->argv[cmd->argc], token);
    cmd->argc++;

    token[0] = '\0';
    while (gettoken(&cmdidx, token, whitespace) != -1 && cmd->argc < MAX_ARGC) {
        cmd->argv[cmd->argc] = _malloc(strlen(token) + 1);
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

    _free(cmd->name);
}

void freepline(line_t *parsedline) {
    freecmd(parsedline->left_cmd);
    freecmd(parsedline->right_cmd);
    _free(parsedline);
}

static void pipe_exec_left(int argc, char *argv[]) {

    function_t function = get_cmd(argv[0]);

    int fd[2];
    if (_create_pipe("shell_pipe", fd) == -1)
        _open_pipe("shell_pipe", fd);
    _sem_post(sem_pipe_exec);

    _dup2(fd[1], 1);
    _close(fd[1]);
    _close(fd[0]);

    function(argc, argv);
}

static void pipe_exec_right(int argc, char *argv[]) {

    function_t function = get_cmd(argv[0]);

    int fd[2];
    _sem_wait(sem_pipe_exec);
    if (_open_pipe("shell_pipe", fd) == -1)
        _create_pipe("shell_pipe", fd);
    _sem_post(sem_pipe_exec);

    _dup2(fd[0], 0);
    _close(fd[0]);
    _close(fd[1]);

    function(argc, argv);
}

static void pipe_exec(cmd_t *left, cmd_t *right) {
    sem_pipe_exec = _sem_open("shell_sem_pipe_exec", 1);
    int left_pid = _run(pipe_exec_left, left->argc, left->argv);
    int right_pid = _run(pipe_exec_right, right->argc, right->argv);
    _waitpid(left_pid, NULL);
    _waitpid(right_pid, NULL);
    _sem_close(sem_pipe_exec);
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

        int pid = -1, proc_status = -1;
        line_t *parsedline = parseline(input);

        switch (parsedline->operator) {
            case '|':
                pipe_exec(parsedline->left_cmd, parsedline->right_cmd);
                break;
            case '&':
                run_command(parsedline->left_cmd->name,
                            parsedline->left_cmd->argc,
                            parsedline->left_cmd->argv);
                break;
            default:
                pid = run_command(parsedline->left_cmd->name,
                                  parsedline->left_cmd->argc,
                                  parsedline->left_cmd->argv);
                _setfg(pid);
                _waitpid(pid, &proc_status);
                break;
        }

        freepline(parsedline);
    }

    return 0;
}
