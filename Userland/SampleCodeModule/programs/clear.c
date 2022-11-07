// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clear.h>
#include <kstdio.h>

#define FULLSCREEN 0

int clear(int argc, char *argv[]) {
    _clear_screen();
    _switch_screen_mode(FULLSCREEN);
}