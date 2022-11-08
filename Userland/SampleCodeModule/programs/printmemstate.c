// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kmman.h>
#include <kstdio.h>
#include <printmemstate.h>

int printmemstate(int argc, char *argv[]) {
    int mem_state[3];
    _get_mem_state(mem_state);
    printf("%15s %15s %10s\n", "TOTAL_MEM", "OCCUPIED_MEM", "FREE_MEM");

    printf("%15d %15d %10d %5s\n", mem_state[0], mem_state[1], mem_state[2],
           "bytes");
    return 0;
}