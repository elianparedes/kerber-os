#include <help.h>
#include <kstdio.h>

static void help_cmds() {
    puts("");
    puts("Available commands:");
    puts("");
    puts(" help        clear          kerberos");
    puts(" time        inforeg        clock");
    puts(" fibonacci   test-inforeg   divzero");
    puts(" primes      printmem       invopcode");
    puts("");
    puts("Type \"help [command]\" for information about a specific command.");
    puts("Try \"help help\".");
    puts("");
}

static void help_all() {
    puts("");
    puts("Commands:");
    puts("    clear           Clears the screen.");
    puts("    time            Displays system day and hour.");
    puts("    clock           Displays real time clock.");
    puts("    fibonacci       Prints fibonacci sequence.");
    puts("    primes          Prints prime numbers.");
    puts("    printmem        Prints memory values from given address.");
    puts("    inforeg         Shows the last snapshot taken.");
    puts("    test-inforeg    Testing program for inforeg.");
    puts("    divzero         Forces divide-by-zero exception.");
    puts("    invopcode       Forces invalid opcode exception.");
    puts("    kerberos        The KerberOS boot screen.");
    puts("");
    puts("Operators:");
    puts("    [c1] | [c2]     Runs both [c1] and [c2] commands in split screen "
         "mode.");
    puts("");
    puts("Type \"help [command]\" for information about a specific command.");
    puts("");
}

static void help_error(char *cmd) {
    printf("Command %s not found.\nType \"help\" to show available commands.\n",
           cmd);
}

static void help_fibonacci() {
    puts("");
    puts("Command:");
    puts("    fibonacci       Prints fibonacci sequence.");
    puts("");
}

static void help_primes() {
    puts("");
    puts("Command:");
    puts("    primes          Prints prime numbers.");
    puts("");
}

static void help_time() {
    puts("");
    puts("Command:");
    puts("    time            Displays system day and hour.");
    puts("");
}

static void help_divzero() {
    puts("");
    puts("Command:");
    puts("    divzero         Forces divide-by-zero exception.");
    puts("");
}

static void help_inforeg() {
    puts("");
    puts("Command:");
    puts("    inforeg         Shows the last snapshot taken.");
    puts("");
    puts("Description:");
    puts("    Prints the value of CPU registers from the last snapshot taken.");
    puts("    by pressing the shortcut [CTRL] + [R], a gray animation");
    puts("    will be shown, and then the snapshot is going to be saved");
    puts("    and ready for future requests.");
    puts("");
}

static void help_invopcode() {
    puts("");
    puts("Command:");
    puts("    invopcode       Forces invalid opcode exception.");
    puts("");
}

static void help_kerberos() {
    puts("");
    puts("Command:");
    puts("    kerberos        The KerberOS boot screen.");
    puts("");
}

static void help_clear() {
    puts("");
    puts("Command:");
    puts("    clear           Clears the screen.");
    puts("");
}

static void help_clock() {
    puts("");
    puts("Command:");
    puts("    clock           Displays real time clock.");
    puts("");
}

static void help_printmem() {
    puts("");
    puts("Command:");
    puts("    fibonacci       Prints fibonacci sequence.");
    puts("");
}

static void help_testinforeg() {
    puts("");
    puts("Command:");
    puts("    test-inforeg    Testing program for inforeg.");
    puts("");
    puts("Description:");
    puts("    Fills CPU registers with dummy values and runs in a infinite "
         "loop");
    puts("    until SIGINT. While looping, the user can take a snapshot for "
         "later use");
    puts("    with inforeg command");
    puts("Note:");
    puts("    Type \"help inforeg\" for more information.");
    puts("");
}

static void help_pipe() {
    puts("");
    puts("Operator:");
    puts("    [c1] | [c2]     Runs both [c1] and [c2] commands in split screen "
         "mode.");
    puts("");
    puts("Synopsis:");
    puts("    [c1] | [c2]");
    puts("    [c1] | [c2] [arg]");
    puts("    [c1] [arg] | [c2] [arg]");
    puts("");
    puts("Description:");
    puts("    Extra spaces between commands, arguments and operator");
    puts("    are always ignored.");
    puts("");
}

void help(char *arg) {
    if (arg == NULL || arg[0] == '\0')
        help_cmds();

    else if (strcmp(arg, "all") == 0)
        help_all();

    else if (strcmp("clear", arg) == 0)
        help_clear();

    else if (strcmp("time", arg) == 0)
        help_time();

    else if (strcmp("clock", arg) == 0)
        help_clock();

    else if (strcmp("fibonacci", arg) == 0)
        help_fibonacci();

    else if (strcmp("primes", arg) == 0)
        help_primes();

    else if (strcmp("printmem", arg) == 0)
        help_printmem();

    else if (strcmp("inforeg", arg) == 0)
        help_inforeg();

    else if (strcmp("test-inforeg", arg) == 0)
        help_testinforeg();

    else if (strcmp("divzero", arg) == 0)
        help_divzero();

    else if (strcmp("invopcode", arg) == 0)
        help_invopcode();

    else if (strcmp("kerberos", arg) == 0)
        help_kerberos();

    else if (strcmp("pipe", arg) == 0)
        help_pipe();

    else
        help_error(arg);
}