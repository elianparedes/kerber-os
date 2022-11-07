// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstdio.h>

void time() {
    time_rtc_t current_time;
    _time(&current_time, 0);
    printf("Current date:\n");
    printf("%d/%d/%d\n", (int)current_time.day, (int)current_time.month,
           (int)current_time.year);
    printf("Current time:\n");
    printf("%d:%d:%d\n", (int)current_time.hour, (int)current_time.minutes,
           (int)current_time.seconds);
}