#include <kstdio.h>

void time(){
    time_t * current_time;
    _time(current_time,0);
    printf("Current date:\n");
    printf("%d/%d/%d\n",current_time->day,current_time->month,current_time->year);
    printf("Current time:\n");
    printf("%d:%d:%d\n",current_time->hour,current_time->minutes,current_time->seconds);
}