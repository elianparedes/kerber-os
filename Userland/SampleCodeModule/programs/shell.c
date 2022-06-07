#include <kstdio.h>
#include <kstring.h>

#define LINE_LENGTH 60
#define TOKEN_LENGTH 128

enum exit_status
{
    SUCCESS = 1,
    FAILURE = 0,
};

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
        buffer[offset++] = c;
    buffer[offset] = '\0';
}

int shell()
{
    char cmd_buff[LINE_LENGTH], token_buff[TOKEN_LENGTH], main_buff[TOKEN_LENGTH], arg_buff[TOKEN_LENGTH];

    while (1)
    {
        read_input(cmd_buff);

        char *input = cmd_buff;
        while (gettoken(&input, token_buff, '|') != -1)
        {
            parse_command(&token_buff, main_buff, arg_buff);
        }
    }

    return 0;
}
