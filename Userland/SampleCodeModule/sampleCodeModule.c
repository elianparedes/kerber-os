// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <help.h>
#include <kctype.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <kstring.h>
#include <shell.h>

#define FULLSCREEN 0

int main() {
    _clear_screen();
    _switch_screen_mode(FULLSCREEN);

    char *args[] = {"shell"};
    _run(shell, 1, args);

    return 0xDEADBEEF;
}