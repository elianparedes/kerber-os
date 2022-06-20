#ifndef _RTC_H_
#define _RTC_H_

#include <stdint.h>

typedef struct time {
    uint64_t year;
    uint64_t month;
    uint64_t day;
    uint64_t hour;
    uint64_t minutes;
    uint64_t seconds;
} time_t;

typedef enum {
    SEC = 0,
    MIN = 2,
    HOUR = 4,
    DAY_WEEK = 6,
    DAY_MONTH = 7,
    MONTH = 8,
    YEAR = 9
} TIME;

typedef enum {
    BCD = 0,
    DECIMAL
} BASE_MODE;

typedef enum {
    HS_12 = 0,
    HS_24
} HS_MODE;

void set_UTC_offset(int utc_offset);

void set_base_mode(BASE_MODE base_mode);

void set_hs_mode(HS_MODE hs_mode);

uint64_t get_seconds();

uint64_t get_minutes();

uint64_t get_hour();

uint64_t get_day_week();

uint64_t get_day_month();

uint64_t get_month();

uint64_t get_year();

void get_struct_time();

#endif /* _RTC_H_ */