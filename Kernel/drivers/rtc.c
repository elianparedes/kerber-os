// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <rtc.h>
#include <stddef.h>

extern uint16_t _rtc_time();
extern void _set_hour12_mode();
extern void _set_hour24_mode();
extern void _set_decimal_mode();
extern void _set_BCD_mode();
// extern void _set_binary_RTC();

static int rtc_utc_offset = 0;
BASE_MODE rtc_base_mode = DECIMAL;
HS_MODE rtc_hs_mode = HS_24;

void set_UTC_offset(int utc_offset) {
    rtc_utc_offset = (utc_offset) % 24;
}

void set_base_mode(BASE_MODE base_mode) {
    rtc_base_mode = base_mode;
    switch (rtc_base_mode) {
        case BCD:
            _set_BCD_mode();
            break;
        case DECIMAL:
            _set_decimal_mode();
            break;
    }
}

void set_hs_mode(HS_MODE hs_mode) {
    rtc_hs_mode = hs_mode;
    switch (rtc_hs_mode) {
        case HS_24:
            _set_hour24_mode();
            break;
        case HS_12:
            _set_hour12_mode();
            break;
        default:
            break;
    }
}

uint64_t get_time(TIME time_type) {
    set_base_mode(rtc_base_mode);
    uint16_t time = _rtc_time(time_type);
    return (uint64_t)time;
}

uint64_t get_seconds() {
    return get_time(SEC);
}

uint64_t get_minutes() {
    return get_time(MIN);
}

uint64_t get_hour() {
    set_hs_mode(rtc_hs_mode);
    uint16_t hour;
    switch (rtc_hs_mode) {
        case HS_12:
        case HS_24:
            hour = get_time(HOUR) + (rtc_utc_offset) + 24;
            return hour % 24;
        default:
            return 0;
    }
}

uint64_t get_day_week() {
    return get_time(DAY_WEEK);
}

uint64_t get_day_month() {
    return get_time(DAY_MONTH);
}

uint64_t get_month() {
    return get_time(MONTH);
}

uint64_t get_year() {
    return get_time(YEAR);
}

void get_struct_time(time_rtc_t *struct_time) {
    struct_time->year = get_year();
    struct_time->month = get_month();
    struct_time->day = get_day_month();
    struct_time->hour = get_hour();
    struct_time->minutes = get_minutes();
    struct_time->seconds = get_seconds();
}
