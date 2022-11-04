#include <printmemstate.h>
#include <kmman.h>
#include <kstdio.h>

void printmemstate(){
    int mem_state[3];
    _get_mem_state(mem_state);
    printf("TOTAL_MEM         OCCUPIED_MEM         FREE_MEM\n");
    printf("%d B         %d B              %d B\n", mem_state[0], mem_state[1], mem_state[2]);
}