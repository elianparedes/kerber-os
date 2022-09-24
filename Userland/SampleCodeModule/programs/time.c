// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>

void time() {
    time_t current_time;
    _time(&current_time, 0);
    printf("Current date:\n");
    printf("%d/%d/%d\n", current_time.day, current_time.month,
           current_time.year);
    printf("Current time:\n");
    printf("%d:%d:%d\n", (uint32_t)current_time.hour, (uint32_t)current_time.minutes,
           (uint32_t)current_time.seconds);
}