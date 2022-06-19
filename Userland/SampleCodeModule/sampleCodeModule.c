/* sampleCodeModule.c */
#include <help.h>
#include <kctype.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <kstring.h>
#include <shell.h>

int main() {
    _clear_screen();

    _switch_screen_mode(0);

    _run(&shell, NULL);

    return 0xDEADBEEF;
}