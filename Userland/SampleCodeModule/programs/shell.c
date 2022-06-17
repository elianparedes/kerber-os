#include <kstdio.h>
#include <kstring.h>
#include <help.h>
#include <primes.h>
#include <fibonacci.h>
#include <time.h>
#include <divzero.h>
#include <invalidopcode.h>

#define LINE_LENGTH 60
#define TOKEN_LENGTH 128

#define PROMPT_SYMBOL "#>"

enum exit_status
{
    SUCCESS = 1,
    FAILURE = 0,
};

static char cntrl_listener;

/**
 * @brief Get token using a state machine.
 * Inspired in John Bode's answer
 *
 * @see https://stackoverflow.com/questions/4547570/tokenizing-a-string-in-c
 */
int gettoken(char **src, char *token, char delimiter)
{
    enum
    {
        START,
        STEP,
        ERROR,
        DONE
    } state = START;

    size_t i = 0;

    if (**src == '\0')
    {
        return -1;
    }

    while (**src != '\0')
    {
        switch (state)
        {
        case START:
            if (**src != delimiter)
            {
                token[i++] = *(*src)++;
                state = STEP;
            }
            else
            {
                *(*src)++;
            }
            break;
        case STEP:
            if (**src != delimiter)
            {
                token[i++] = *(*src)++;
            }
            else
            {
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

int parse_command(char **src, char *main, char *arg)
{
    enum
    {
        START,
        COMMAND,
        ARGUMENT,
        ERROR,
        DONE
    } state = START;

    char token[TOKEN_LENGTH];

    while (1)
    {
        int res = gettoken(&src, token, ' ');
        if (res == SUCCESS)
        {
            switch (state)
            {
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
        }
        else if (res == -1)
            break;
    }
    return SUCCESS;
}

void read_input(char *buffer)
{
    char c;
    int offset = 0;
    while ((c = getchar()) != '\n')
    {   
        putchar(c);
        buffer[offset++] = c;
    }
    buffer[offset] = '\0';
    printf("\n");
}

void run_command(char *main)
{
    if (strcmp(main, "help") == 0)
        _run(&help);
    else if (strcmp(main, "fibonacci") == 0)
        _run(&fibonacci);
    else if (strcmp(main, "primes") == 0)
        _run(&primes);
    else if (strcmp(main, "time") == 0)
         _run(&time);
    else if (strcmp(main, "divzero") == 0)
        _run(&divzero);
    else if (strcmp(main, "invalidopcode") == 0)
        _run(&invalidopcode);
    else
        return;
}

int shell()
{
    char cmd_buff[LINE_LENGTH], token_buff[TOKEN_LENGTH];

    _cntrl_listener(&cntrl_listener);
    while (1)
    {
        read_input(cmd_buff);

        char *input = cmd_buff;
        int p_count = 0;

        char main_buff1[TOKEN_LENGTH], arg_buff1[TOKEN_LENGTH];
        char main_buff2[TOKEN_LENGTH], arg_buff2[TOKEN_LENGTH];

        while (gettoken(&input, token_buff, '|') != -1)
        {
            if (p_count == 0)
            {
                parse_command(&token_buff, main_buff1, arg_buff1);
                p_count++;
            }
            else if (p_count == 1)
            {
                parse_command(&token_buff, main_buff2, arg_buff2);
                p_count++;
            }
        }

        /**
         * arguments not supported yet
         */

        if (p_count == 1)
        {
            _switch_screen_mode(0);
            run_command(main_buff1);
        }
        else if (p_count == 2)
        {
            _switch_screen_mode(1);
            run_command(main_buff1);
            run_command(main_buff2);
        }
        else
            continue;
    }

    return 0;
}
