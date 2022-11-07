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
} time_rtc_t;

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

/**
 * @brief Set the UTC offset of RTC
 *
 * @param utc_offset
 */
void set_UTC_offset(int utc_offset);

/**
 * @brief Set the base mode of RTC
 *
 * @param base_mode
 */
void set_base_mode(BASE_MODE base_mode);

/**
 * @brief Set the display mode from RTC.
 *
 * @param hs_mode the clock format. Either HS_12 or HS_24
 */
void set_hs_mode(HS_MODE hs_mode);

/**
 * @brief Get the seconds from RTC
 *
 * @return uint64_t seconds
 */
uint64_t get_seconds();

/**
 * @brief Get the minutes from RTC
 *
 * @return uint64_t minutes
 */
uint64_t get_minutes();

/**
 * @brief Get the hours from RTC
 *
 * @return uint64_t hours
 */
uint64_t get_hour();

/**
 * @brief Get the number of day in week from RTC
 *
 * @return uint64_t number of day of the week
 */
uint64_t get_day_week();

/**
 * @brief Get the number of day in month from RTC
 *
 * @return uint64_t number of day in month
 */
uint64_t get_day_month();

/**
 * @brief Get the number of month from RTC
 *
 * @return uint64_t month number
 */
uint64_t get_month();

/**
 * @brief Get the year from RTC
 *
 * @return uint64_t year
 */
uint64_t get_year();

/**
 * @brief Get the struct time object
 *
 */
void get_struct_time();

#endif /* _RTC_H_ */