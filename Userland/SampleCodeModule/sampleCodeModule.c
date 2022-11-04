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
    _run(shell, 0, NULL);

    return 0xDEADBEEF;
}