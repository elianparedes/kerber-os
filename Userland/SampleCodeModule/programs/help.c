// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <help.h>
#include <kstdio.h>
#include <kstring.h>

static void formatter(char *name1, char *name2, char *name3, char *name4) {
    printf(" %15s    %15s    %15s    %15s\n", name1, name2, name3, name4);
}

static void help_cmds() {
    puts("");
    puts("Available commands:");
    formatter("clear", "time", "fibonacci", "primes");
    formatter("ps", "mem", "sem", "kill");
    formatter("nice", "block", "cat", "wc");
    formatter("filter", "pipe", "phylo", "loop");
    formatter("printmem", "inforeg", "divzero", "invopcode");
    puts(" kerberos");
    puts("");
    puts("Tests:");
    formatter("testmm", "testprocesses", "testpriority", "testsync");
    formatter("test-inforeg", "runalltests", "", "");
    puts("");
    puts("Operators:");
    puts(" [p1] | [p2]     Connects a pipe between [p1] and [p2] processes.");
    puts(" [p1] &          Runs process [p1] on background.");
    puts("");
    puts("Type \"help [command]\" for information about a specific command.");
    puts("");
}

static void help_all() {
    puts("");
    puts("Commands:");
    puts("    clear           Clears the screen.");
    puts("    time            Displays system day and hour.");
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
    puts("    [c1] | [c2] [argv[0]]");
    puts("    [c1] [argv[0]] | [c2] [argv[0]]");
    puts("");
    puts("Description:");
    puts("    Extra spaces between commands, arguments and operator");
    puts("    are always ignored.");
    puts("");
}

int help(int argc, char *argv[]) {
    if (argc == 1)
        help_cmds();

    else if (strcmp("clear", argv[1]) == 0)
        help_clear();

    else if (strcmp("time", argv[1]) == 0)
        help_time();

    else if (strcmp("fibonacci", argv[1]) == 0)
        help_fibonacci();

    else if (strcmp("primes", argv[1]) == 0)
        help_primes();

    else if (strcmp("printmem", argv[1]) == 0)
        help_printmem();

    else if (strcmp("inforeg", argv[1]) == 0)
        help_inforeg();

    else if (strcmp("test-inforeg", argv[1]) == 0)
        help_testinforeg();

    else if (strcmp("divzero", argv[1]) == 0)
        help_divzero();

    else if (strcmp("invopcode", argv[1]) == 0)
        help_invopcode();

    else if (strcmp("kerberos", argv[1]) == 0)
        help_kerberos();

    else if (strcmp("pipe", argv[1]) == 0)
        help_pipe();

    else
        help_error(argv[1]);

    return 0;
}