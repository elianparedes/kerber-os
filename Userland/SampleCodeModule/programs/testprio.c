// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>
#include <stdint.h>
#include <test_util.h>

#define MINOR_WAIT                                                             \
    1000000 // Change this value to prevent a process from flooding the
            // screen
#define WAIT                                                                   \
    10000000 // TChange this value to make the wait long enough to see
             // theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST          1  // Change as required
#define MEDIUM          5  // Change as required
#define HIGHEST         10 // Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

int test_prio(int argc, char const *argv[]) {
    while (1) {
        int64_t pids[TOTAL_PROCESSES];
        char *args[] = {"endless"};
        uint64_t i;

        for (i = 0; i < TOTAL_PROCESSES; i++)
            pids[i] = _run(endless_loop_print, 1, args);

        bussy_wait(WAIT);
        printf("\nCHANGING PRIORITIES...\n");

        for (i = 0; i < TOTAL_PROCESSES; i++)
            _set_priority(pids[i], prio[i]);

        bussy_wait(WAIT);
        printf("\nBLOCKING...\n");

        for (i = 0; i < TOTAL_PROCESSES; i++)
            _block(pids[i]);

        printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

        for (i = 0; i < TOTAL_PROCESSES; i++)
            _set_priority(pids[i], MEDIUM);

        printf("UNBLOCKING...\n");

        for (i = 0; i < TOTAL_PROCESSES; i++)
            _unblock(pids[i]);

        bussy_wait(WAIT);
        printf("\nKILLING...\n");

        for (i = 0; i < TOTAL_PROCESSES; i++) {
            _kill(pids[i]);
            _wait();
        }
    }
}