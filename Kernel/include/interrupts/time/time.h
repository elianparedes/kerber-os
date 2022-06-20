#ifndef _TIME_H_
#define _TIME_H_

/**
 * @brief Increments the ticks count. 
 * 
 */
void timer_handler();

/**
 * @brief Get how many ticks passed from timer's start.
 * 
 * @return int ticks count
 */
int ticks_elapsed();

/**
 * @brief Get how many seconds passed from timer's start.
 * 
 * @return int seconds
 */
int seconds_elapsed();

#endif