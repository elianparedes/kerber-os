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